/* doubango tinyIPsec plugin for Linux
 *
 * Copyright (C) 2021 Harald Welte <laforge@osmocom.org>
 *
 * DOUBANGO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DOUBANGO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DOUBANGO.
 */

#include "tipsec.h"
#include "netlink_xfrm.h"

#include "tsk_memory.h"
#include "tsk_object.h"
#include "tsk_debug.h"
#include "tsk_plugin.h"

#include <arpa/inet.h>

#define LOGTIC(ctx, fmt, args...) \
	fprintf(stderr, "LINUX_IPSEC: (%p) %s: " fmt, ctx, __func__, ## args)

typedef struct plugin_linux_ipsec_ctx_s {
	TIPSEC_DECLARE_CTX;

	tipsec_ctx_t* pc_base;
	/* any linux-specific state structure listed below; so far none */
} plugin_linux_ipsec_ctx_t;

static struct mnl_socket *g_mnl_s;

/***********************************************************************
 * Private functions
 ***********************************************************************/

static void sockaddr_from4(struct sockaddr_storage *out, const struct in_addr *ia4, uint16_t port)
{
	struct sockaddr_in *sa4 = (struct sockaddr_in *) out;

	memset(sa4, 0, sizeof(*sa4));
	sa4->sin_family = AF_INET;
	sa4->sin_addr = *ia4;
	sa4->sin_port = htons(port);
}

static void sockaddr_from6(struct sockaddr_storage *out, const struct in6_addr *ia6, uint16_t port)
{
	struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *) out;

	memset(sa6, 0, sizeof(*sa6));
	sa6->sin6_family = AF_INET6;
	sa6->sin6_addr = *ia6;
	sa6->sin6_port = htons(port);
}

/* convert internal state to 4x sockaddr */
static void gen_sockaddrs(struct sockaddr_storage *out_uc, struct sockaddr_storage *out_us,
			  struct sockaddr_storage *out_pc, struct sockaddr_storage *out_ps,
			  const tipsec_ctx_t *_p_ctx)
{
	if (_p_ctx->use_ipv6) {
		sockaddr_from6(out_uc, _p_ctx->addr_local, _p_ctx->port_uc);
		sockaddr_from6(out_us, _p_ctx->addr_local, _p_ctx->port_us);
		sockaddr_from6(out_pc, _p_ctx->addr_remote, _p_ctx->port_pc);
		sockaddr_from6(out_ps, _p_ctx->addr_remote, _p_ctx->port_ps);
	} else {
		sockaddr_from4(out_uc, _p_ctx->addr_local, _p_ctx->port_uc);
		sockaddr_from4(out_us, _p_ctx->addr_local, _p_ctx->port_us);
		sockaddr_from4(out_pc, _p_ctx->addr_remote, _p_ctx->port_pc);
		sockaddr_from4(out_ps, _p_ctx->addr_remote, _p_ctx->port_ps);
	}
}

/***********************************************************************
 * Call-Back functions of tipsec core
 ***********************************************************************/

static tipsec_error_t
_plugin_linux_ipsec_ctx_init(tipsec_ctx_t *_p_ctx)
{
	plugin_linux_ipsec_ctx_t *p_ctx = (plugin_linux_ipsec_ctx_t *) _p_ctx;

	if (p_ctx->pc_base->initialized) {
		TSK_DEBUG_ERROR("Already initialized");
		return tipsec_error_invalid_state;
	}

	/* FIXME */

	p_ctx->pc_base->initialized = tsk_true;
	p_ctx->pc_base->state = tipsec_state_initial;

	return tipsec_error_success;
}

/* SIP stack tells us about local IPs/Ports and asks us to allocate SPIs */
static tipsec_error_t
_plugin_linux_ipsec_ctx_set_local(tipsec_ctx_t *_p_ctx, const char *addr_local, const char *addr_remote, tipsec_port_t port_uc, tipsec_port_t port_us)
{
	//plugin_linux_ipsec_ctx_t *p_ctx = (plugin_linux_ipsec_ctx_t *) _p_ctx;
	struct sockaddr_storage sa_local, sa_remote;
	int rc;

	LOGTIC(_p_ctx, "%s:%u+%u -> %s\n", addr_local, port_uc, port_us, addr_remote);

	_p_ctx->addr_local = tsk_realloc(_p_ctx->addr_local, _p_ctx->use_ipv6 ? 16 : 4);
	if (!_p_ctx->addr_local)
        	return tipsec_error_outofmemory;

	_p_ctx->addr_remote = tsk_realloc(_p_ctx->addr_remote, _p_ctx->use_ipv6 ? 16 : 4);
	if (!_p_ctx->addr_remote)
		return tipsec_error_outofmemory;

	if (_p_ctx->use_ipv6) {
		if (inet_pton(AF_INET6, addr_local, _p_ctx->addr_local) != 1)
			return tipsec_error_sys;
		sockaddr_from6(&sa_local, _p_ctx->addr_local, 0);
		if (inet_pton(AF_INET6, addr_remote, _p_ctx->addr_remote) != 1)
			return tipsec_error_sys;
		sockaddr_from6(&sa_remote, _p_ctx->addr_remote, 0);
	} else {
		if (inet_pton(AF_INET, addr_local, _p_ctx->addr_local) != 1)
			return tipsec_error_sys;
		sockaddr_from4(&sa_local, _p_ctx->addr_local, 0);
		if (inet_pton(AF_INET, addr_remote, _p_ctx->addr_remote) != 1)
			return tipsec_error_sys;
		sockaddr_from4(&sa_remote, _p_ctx->addr_remote, 0);
#if 0
		/* FIXME: do we really need those in host byte order? */
		*((uint32_t *)_p_ctx->addr_local) = ntohl(*(uint32_t *)_p_ctx->addr_local);
		*((uint32_t *)_p_ctx->addr_remote) = ntohl(*(uint32_t *)_p_ctx->addr_remote);
#endif
	}

	_p_ctx->port_uc = port_uc;
	_p_ctx->port_us = port_us;

	/* we need to allocate local SPIs here, one for TCP client and one for TCP server role.
	 * These will be passed to the P-CSCF in the Security-Client header. */
	rc = xfrm_spi_alloc(g_mnl_s, 1, &_p_ctx->spi_uc, (struct sockaddr *)&sa_local, (struct sockaddr *)&sa_remote);
	if (rc != 0)
		return tipsec_error_sys;
	rc = xfrm_spi_alloc(g_mnl_s, 2, &_p_ctx->spi_us, (struct sockaddr *)&sa_local, (struct sockaddr *)&sa_remote);
	if (rc != 0)
		return tipsec_error_sys;

	_p_ctx->state = tipsec_state_inbound;

	return tipsec_error_success;
}

/* SIP Stack informs us about the remote SPIs + TCP ports */
static tipsec_error_t
_plugin_linux_ipsec_ctx_set_remote(tipsec_ctx_t *_p_ctx, tipsec_spi_t spi_pc, tipsec_spi_t spi_ps, tipsec_port_t port_pc, tipsec_port_t port_ps, tipsec_lifetime_t lifetime)
{
	//plugin_linux_ipsec_ctx_t *p_ctx = (plugin_linux_ipsec_ctx_t *) _p_ctx;

	LOGTIC(_p_ctx, "SPI_PC=0x%08x SPI_PS=0x%08x PORT_PC=%u, PORT_PS=%u lifetime=%lu\n",
		spi_pc, spi_ps, port_pc, port_ps, lifetime);

	_p_ctx->lifetime = lifetime;

	_p_ctx->port_ps = port_ps;
	_p_ctx->port_pc = port_pc;

	_p_ctx->spi_ps = spi_ps;
	_p_ctx->spi_pc = spi_pc;

	/* we cannot yet create the SAs as we don't have the keys yet */

	_p_ctx->state = tipsec_state_full;

	return tipsec_error_success;
}

/* SIP stack informs us about the key material (obtained from SIM after '401 Unauthorized' with RAND+AUTN */
static tipsec_error_t
_plugin_linux_ipsec_ctx_set_keys(tipsec_ctx_t *_p_ctx, const tipsec_key_t *ik, const tipsec_key_t *ck)
{
	//plugin_linux_ipsec_ctx_t *p_ctx = (plugin_linux_ipsec_ctx_t *) _p_ctx;

	LOGTIC(_p_ctx, "entered\n");

	_p_ctx->ik = tsk_realloc(_p_ctx->ik, TIPSEC_KEY_LEN);
	if (!_p_ctx->ik)
		return tipsec_error_outofmemory;
	memcpy(_p_ctx->ik, ik, TIPSEC_KEY_LEN);

	_p_ctx->ck = tsk_realloc(_p_ctx->ck, TIPSEC_KEY_LEN);
	if (!_p_ctx->ck)
		return tipsec_error_outofmemory;
	memcpy(_p_ctx->ck, ck, TIPSEC_KEY_LEN);

	return tipsec_error_success;
}

/* SIP stack asks us to start the IPsec processing */
static tipsec_error_t
_plugin_linux_ipsec_ctx_start(tipsec_ctx_t *_p_ctx)
{
	//plugin_linux_ipsec_ctx_t *p_ctx = (plugin_linux_ipsec_ctx_t *) _p_ctx;
	struct sockaddr_storage uc_saddr, us_saddr, pc_saddr, ps_saddr;
	struct xfrm_algobuf auth, ciph;
	int rc;

	LOGTIC(_p_ctx, "entered\n");

	memset(&auth, 0, sizeof(auth));
	memset(&ciph, 0, sizeof(ciph));

	/* build sockaddrs from the internal representations */
	gen_sockaddrs(&uc_saddr, &us_saddr, &pc_saddr, &ps_saddr, _p_ctx);

	/* build cipher specs from internal representations */
	switch (_p_ctx->alg) {
	case tipsec_alg_hmac_md5_96:
		strcpy(auth.algo.alg_name, "md5");
		break;
	case tipsec_alg_hmac_sha_1_96:
		strcpy(auth.algo.alg_name, "sha1");
		break;
	default:
		LOGTIC(_p_ctx, "Unsupported authentication algorithm %d\n", _p_ctx->alg);
		return tipsec_error_notimplemented;
	}
	auth.algo.alg_key_len = TIPSEC_KEY_LEN * 8;
	memcpy(auth.algo.alg_key, _p_ctx->ik, TIPSEC_KEY_LEN);

	switch (_p_ctx->ealg) {
	case tipsec_ealg_null:
		strcpy(ciph.algo.alg_name, "cipher_null");
		break;
	case tipsec_ealg_aes:
		strcpy(ciph.algo.alg_name, "aes");
		ciph.algo.alg_key_len = TIPSEC_KEY_LEN * 8;
		memcpy(ciph.algo.alg_key, _p_ctx->ck, TIPSEC_KEY_LEN);
		break;
	case tipsec_ealg_des_ede3_cbc:
		strcpy(ciph.algo.alg_name, "des3_ede");
		ciph.algo.alg_key_len = 192;
		memcpy(ciph.algo.alg_key, _p_ctx->ck, TIPSEC_KEY_LEN);
		memcpy(ciph.algo.alg_key+16, _p_ctx->ck, 8);
		break;
	default:
		LOGTIC(_p_ctx, "Unsupported encryption algorithm %d\n", _p_ctx->ealg);
		return tipsec_error_notimplemented;
	}

	/* actually create the SAs and policies in the kernel */

	/* UE client to P-CSCF server */
	rc = xfrm_sa_add(g_mnl_s, _p_ctx->spi_ps, (struct sockaddr *) &uc_saddr, (struct sockaddr *) &ps_saddr,
			 _p_ctx->spi_ps, &auth.algo, &ciph.algo);
	if (rc < 0)
		return tipsec_error_sys;

	rc = xfrm_policy_add(g_mnl_s, (struct sockaddr *) &uc_saddr, (struct sockaddr *) &ps_saddr,
			     _p_ctx->spi_ps, false);
	if (rc < 0)
		goto del_sa_1;

	/* P-CSCF client to UE server */
	rc = xfrm_sa_add(g_mnl_s, _p_ctx->spi_us, (struct sockaddr *) &pc_saddr, (struct sockaddr *) &us_saddr,
			 _p_ctx->spi_us, &auth.algo, &ciph.algo);
	if (rc < 0)
		goto del_policy_1;
	rc = xfrm_policy_add(g_mnl_s, (struct sockaddr *) &pc_saddr, (struct sockaddr *) &us_saddr,
			     _p_ctx->spi_us, true);
	if (rc < 0)
		goto del_sa_2;

	/* P-CSCF server to UE client */
	rc = xfrm_sa_add(g_mnl_s, _p_ctx->spi_uc, (struct sockaddr *) &ps_saddr, (struct sockaddr *) &uc_saddr,
			 _p_ctx->spi_uc, &auth.algo, &ciph.algo);
	if (rc < 0)
		goto del_policy_2;
	rc = xfrm_policy_add(g_mnl_s, (struct sockaddr *) &ps_saddr, (struct sockaddr *) &uc_saddr,
			     _p_ctx->spi_uc, true);
	if (rc < 0)
		goto del_sa_3;

	/* UE server to P-CSCF client */
	rc = xfrm_sa_add(g_mnl_s, _p_ctx->spi_pc, (struct sockaddr *) &us_saddr, (struct sockaddr *) &pc_saddr,
			 _p_ctx->spi_pc, &auth.algo, &ciph.algo);
	if (rc < 0)
		goto del_policy_3;
	rc = xfrm_policy_add(g_mnl_s, (struct sockaddr *) &us_saddr, (struct sockaddr *) &pc_saddr,
			     _p_ctx->spi_pc, false);
	if (rc < 0)
		goto del_sa_4;

	_p_ctx->state = tipsec_state_active;
	_p_ctx->started = 1;

	return tipsec_error_success;

	/* clean-up in case of failure */
del_sa_4:
	xfrm_sa_del(g_mnl_s, (struct sockaddr *) &us_saddr, (struct sockaddr *) &pc_saddr, _p_ctx->spi_pc);
del_policy_3:
	xfrm_policy_del(g_mnl_s, (struct sockaddr *) &ps_saddr, (struct sockaddr *) &uc_saddr, true);
del_sa_3:
	xfrm_sa_del(g_mnl_s, (struct sockaddr *) &ps_saddr, (struct sockaddr *) &uc_saddr, _p_ctx->spi_uc);
del_policy_2:
	xfrm_policy_del(g_mnl_s, (struct sockaddr *) &pc_saddr, (struct sockaddr *) &us_saddr, true);
del_sa_2:
	xfrm_sa_del(g_mnl_s, (struct sockaddr *) &pc_saddr, (struct sockaddr *) &us_saddr, _p_ctx->spi_us);
del_policy_1:
	xfrm_policy_del(g_mnl_s, (struct sockaddr *) &uc_saddr, (struct sockaddr *) &ps_saddr, false);
del_sa_1:
	xfrm_sa_del(g_mnl_s, (struct sockaddr *) &uc_saddr, (struct sockaddr *) &ps_saddr, _p_ctx->spi_ps);

	return tipsec_error_sys;
}

/* SIP stack asks us to stop the IPsec processing */
static tipsec_error_t
_plugin_linux_ipsec_ctx_stop(tipsec_ctx_t *_p_ctx)
{
	//plugin_linux_ipsec_ctx_t *p_ctx = (plugin_linux_ipsec_ctx_t *) _p_ctx;
	struct sockaddr_storage uc_saddr, us_saddr, pc_saddr, ps_saddr;

	LOGTIC(_p_ctx, "entered\n");

	/* build sockaddrs from the internal representations */
	gen_sockaddrs(&uc_saddr, &us_saddr, &pc_saddr, &ps_saddr, _p_ctx);

	/* remove the SAs and policies from the kernel */
	xfrm_policy_del(g_mnl_s, (struct sockaddr *) &us_saddr, (struct sockaddr *) &pc_saddr, false);
	xfrm_sa_del(g_mnl_s, (struct sockaddr *) &us_saddr, (struct sockaddr *) &pc_saddr, _p_ctx->spi_pc);

	xfrm_policy_del(g_mnl_s, (struct sockaddr *) &ps_saddr, (struct sockaddr *) &uc_saddr, true);
	xfrm_sa_del(g_mnl_s, (struct sockaddr *) &ps_saddr, (struct sockaddr *) &uc_saddr, _p_ctx->spi_uc);

	xfrm_policy_del(g_mnl_s, (struct sockaddr *) &pc_saddr, (struct sockaddr *) &us_saddr, true);
	xfrm_sa_del(g_mnl_s, (struct sockaddr *) &pc_saddr, (struct sockaddr *) &us_saddr, _p_ctx->spi_us);

	xfrm_policy_del(g_mnl_s, (struct sockaddr *) &uc_saddr, (struct sockaddr *) &ps_saddr, false);
	xfrm_sa_del(g_mnl_s, (struct sockaddr *) &uc_saddr, (struct sockaddr *) &ps_saddr, _p_ctx->spi_ps);

	_p_ctx->started = 0;
	_p_ctx->state = tipsec_state_initial;

	return tipsec_error_success;
}


/***********************************************************************
 * tipsec Plugin Definition
 ***********************************************************************/

static tsk_object_t *_plugin_linux_ipsec_ctx_ctor(tsk_object_t *self, va_list *app)
{
	plugin_linux_ipsec_ctx_t *p_ctx = (plugin_linux_ipsec_ctx_t *) self;

	if (p_ctx)
		p_ctx->pc_base = TIPSEC_CTX(p_ctx);

	g_mnl_s = xfrm_init_mnl_socket();
	LOGTIC(p_ctx, "context created\n");

	return self;
}

static tsk_object_t *_plugin_linux_ipsec_ctx_dtor(tsk_object_t *self)
{
	plugin_linux_ipsec_ctx_t *p_ctx = (plugin_linux_ipsec_ctx_t *) self;

	if (!p_ctx)
		return self;

	if (p_ctx->pc_base->started)
		tipsec_ctx_stop(p_ctx->pc_base);

	/* FIXME */

	TSK_FREE(p_ctx->pc_base->addr_local);
	TSK_FREE(p_ctx->pc_base->addr_remote);

	TSK_FREE(p_ctx->pc_base->ik);
	TSK_FREE(p_ctx->pc_base->ck);

	LOGTIC(p_ctx, "context destroyed\n");

	return self;
}

/* object definition */
static const tsk_object_def_t plugin_linux_ipsec_ctx_def_s = {
	sizeof(plugin_linux_ipsec_ctx_t),
	_plugin_linux_ipsec_ctx_ctor,
	_plugin_linux_ipsec_ctx_dtor,
	tsk_null,
};

/* plugin definition */
static const tipsec_plugin_def_t plugin_linux_ipsec_plugin_def_s = {
	&plugin_linux_ipsec_ctx_def_s,
	tipsec_impl_type_ltools,
	"Linux kernel IPSec",
	_plugin_linux_ipsec_ctx_init,
	_plugin_linux_ipsec_ctx_set_local,
	_plugin_linux_ipsec_ctx_set_remote,
	_plugin_linux_ipsec_ctx_set_keys,
	_plugin_linux_ipsec_ctx_start,
	_plugin_linux_ipsec_ctx_stop,
};
//static const tipsec_plugin_def_t *plugin_win_ipsec_vista_plugin_def_t = &plugin_win_ipsec_vista_plugin_def_s;

/***********************************************************************
 * core Plugin Definition
 ***********************************************************************/

int __plugin_get_def_count()
{
	return 1;
}

tsk_plugin_def_type_t __plugin_get_def_type_at(int index)
{
	switch (index) {
	case 0:
		return tsk_plugin_def_type_ipsec;
	default:
		TSK_DEBUG_ERROR("No plugin at index %d", index);
		return tsk_plugin_def_type_none;
	}
}

tsk_plugin_def_media_type_t __plugin_get_def_media_type_at(int index)
{
	switch (index) {
	case 0:
		return tsk_plugin_def_media_type_all;
	default:
		TSK_DEBUG_ERROR("No plugin at index %d", index);
		return tsk_plugin_def_media_type_none;
	}
}

tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index)
{
	switch (index) {
	case 0:
		return &plugin_linux_ipsec_plugin_def_s;
	default:
		TSK_DEBUG_ERROR("No plugin at index %d", index);
		return tsk_null;
	}
}
