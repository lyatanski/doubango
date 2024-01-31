/* Linux kernel IPsec interfacing via netlink XFRM
 *
 * Copyright (C) 2021 Harald Welte <laforge@osmocom.org>
 *
 * DOUBANGO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>

#include <libmnl/libmnl.h>
#include <linux/xfrm.h>
#include <arpa/inet.h>

#include "netlink_xfrm.h"

#define XFRM_USER_ID	0x240299 /* some random number; let's use TS 24.299 */

struct mnl_socket *xfrm_init_mnl_socket(void)
{
	struct mnl_socket *mnl_socket = mnl_socket_open(NETLINK_XFRM);
	if (!mnl_socket) {
		fprintf(stderr, "ERR: Could not open XFRM netlink socket: %s", strerror(errno));
		return NULL;
	}

	if (mnl_socket_bind(mnl_socket, 0, MNL_SOCKET_AUTOPID) < 0) {
		fprintf(stderr, "ERR: Could not open XFRM netlink socket: %s", strerror(errno));
		mnl_socket_close(mnl_socket);
		return NULL;
	}

	return mnl_socket;
}

static unsigned int get_next_nlmsg_seq(void)
{
	static unsigned int next_seq;
	return next_seq++;
}


/* this is just a simple call-back which returns the nlmsghdr via 'data' */
static int data_cb(const struct nlmsghdr *nlh, void *data)
{
	const struct nlmsghdr **rx = data;

	*rx = nlh;

	/* FIXME: is there a situation in which we'd want to return OK and not STOP? */
	return MNL_CB_STOP;
}

/* send 'tx' via 'mnl_sock' and receive messages from kernel, using caller-provided
 * rx_buf/rx_buf_size as temporary storage buffer; return response nlmsghdr in 'rx' */
static int transceive_mnl(struct mnl_socket *mnl_sock, const struct nlmsghdr *tx,
			  uint8_t *rx_buf, size_t rx_buf_size, struct nlmsghdr **rx)
{
	int rc;

	rc = mnl_socket_sendto(mnl_sock, tx, tx->nlmsg_len);
	if (rc < 0) {
		fprintf(stderr, "ERR: cannot create IPsec SA: %s\n", strerror(errno));
		return -1;
	}

	/* iterate until it is our answer, handing to mnl_cb_run, ... */
	while (1) {
		rc = mnl_socket_recvfrom(mnl_sock, rx_buf, rx_buf_size);
		if (rc == -1) {
			perror("mnl_socket_recvfrom");
			return -EIO;
		}

		rc = mnl_cb_run(rx_buf, rc, tx->nlmsg_seq, mnl_socket_get_portid(mnl_sock), data_cb, rx);
		if (rc == -1) {
			perror("mnl_cb_run");
			return -EIO;
		} else if (rc <= MNL_CB_STOP)
			break;
	}

	return 0;
}

static int sockaddrs2xfrm_sel(struct xfrm_selector *sel, const struct sockaddr *src,
			      const struct sockaddr *dst)
{
	const struct sockaddr_in *sin;
	const struct sockaddr_in6 *sin6;

	switch (src->sa_family) {
	case AF_INET:
		sin = (const struct sockaddr_in *) src;
		sel->saddr.a4 = sin->sin_addr.s_addr;
		sel->prefixlen_s = 32;
		sel->sport = sin->sin_port;
		sin = (const struct sockaddr_in *) dst;
		sel->daddr.a4 = sin->sin_addr.s_addr;
		sel->prefixlen_d = 32;
		sel->dport = sin->sin_port;
		break;
	case AF_INET6:
		sin6 = (const struct sockaddr_in6 *) src;
		memcpy(sel->saddr.a6, &sin6->sin6_addr, sizeof(sel->saddr.a6));
		sel->prefixlen_s = 128;
		sel->sport = sin6->sin6_port;
		sin6 = (const struct sockaddr_in6 *) dst;
		memcpy(sel->daddr.a6, &sin6->sin6_addr, sizeof(sel->daddr.a6));
		sel->prefixlen_d = 128;
		sel->dport = sin6->sin6_port;
		break;
	default:
		return -EINVAL;
	}
	sel->dport_mask = 0xffff;
	sel->sport_mask = 0xffff;
	sel->family = src->sa_family;

	return 0;
}


/***********************************************************************
 * SPI Allocation
 ***********************************************************************/

/* allocate a local SPI for ESP between given src+dst address */
int xfrm_spi_alloc(struct mnl_socket *mnl_sock, uint32_t reqid, uint32_t *spi_out,
		   const struct sockaddr *src, const struct sockaddr *dst)
{
	uint8_t msg_buf[MNL_SOCKET_BUFFER_SIZE];
	uint8_t rx_buf[MNL_SOCKET_BUFFER_SIZE];
	struct xfrm_userspi_info *xui, *rx_xui;
	struct nlmsghdr *nlh, *rx_nlh = NULL;
	const struct sockaddr_in *sin;
	const struct sockaddr_in6 *sin6;
	int rc;

	memset(msg_buf, 0, sizeof(msg_buf));

	if (src->sa_family != dst->sa_family)
		return -EINVAL;

	nlh = mnl_nlmsg_put_header(msg_buf);
	nlh->nlmsg_flags = NLM_F_REQUEST,
	nlh->nlmsg_type = XFRM_MSG_ALLOCSPI,
	nlh->nlmsg_seq = get_next_nlmsg_seq();
	//nlh->nlmsg_pid = reqid; //FIXME

	xui = (struct xfrm_userspi_info *) mnl_nlmsg_put_extra_header(nlh, sizeof(*xui));

	xui->info.family = src->sa_family;

	/* RFC4303 reserves 0..255 */
	xui->min = 0x100;
	xui->max = 0xffffffff;

	/* ID src, dst, proto */
	switch (src->sa_family) {
	case AF_INET:
		sin = (const struct sockaddr_in *) src;
		printf("src=%s ", inet_ntoa(sin->sin_addr));
		xui->info.saddr.a4 = sin->sin_addr.s_addr;
		sin = (const struct sockaddr_in *) dst;
		printf("dst=%s ", inet_ntoa(sin->sin_addr));
		xui->info.id.daddr.a4 = sin->sin_addr.s_addr;
		//xui->info.sel.prefixlen_d = 32;
		break;
	case AF_INET6:
		sin6 = (const struct sockaddr_in6 *) src;
		memcpy(xui->info.saddr.a6, &sin6->sin6_addr, sizeof(xui->info.saddr.a6));
		//xui->info.sel.prefixlen_s = 128;
		sin6 = (const struct sockaddr_in6 *) dst;
		memcpy(xui->info.id.daddr.a6, &sin6->sin6_addr, sizeof(xui->info.id.daddr.a6));
		//xui->info.sel.prefixlen_d = 128;
		break;
	default:
		fprintf(stderr, "ERR: unsupported address family %u\n", src->sa_family);
		return -1;
	}

	xui->info.id.proto = IPPROTO_ESP;
	xui->info.reqid = reqid;
	xui->info.mode = XFRM_MODE_TRANSPORT;
	//xui->info.replay_window = 32; // TODO: check spec

	rc = transceive_mnl(mnl_sock, nlh, rx_buf, MNL_SOCKET_BUFFER_SIZE, &rx_nlh);
	if (rc < 0) {
		fprintf(stderr, "ERR: cannot create IPsec SA: %s\n", strerror(errno));
		return -1;
	}

	/* parse response */
	rx_xui = (void *)rx_nlh + sizeof(*rx_nlh);
	//printf("Allocated SPI=0x%08x\n", ntohl(xui->info.id.spi));
	*spi_out = ntohl(rx_xui->info.id.spi);

	return 0;
}

/***********************************************************************
 * SA (Security Association)
 ***********************************************************************/

int xfrm_sa_del(struct mnl_socket *mnl_sock,
		const struct sockaddr *src, const struct sockaddr *dst, uint32_t spi)
{
	uint8_t msg_buf[MNL_SOCKET_BUFFER_SIZE];
	uint8_t rx_buf[MNL_SOCKET_BUFFER_SIZE];
	struct xfrm_usersa_id *said;
	struct nlmsghdr *nlh, *rx_nlh;
	const struct sockaddr_in *sin;
	const struct sockaddr_in6 *sin6;
	xfrm_address_t saddr;
	int rc;

	memset(&saddr, 0, sizeof(saddr));

	nlh = mnl_nlmsg_put_header(msg_buf);
	nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK;
	nlh->nlmsg_type = XFRM_MSG_DELSA;
	nlh->nlmsg_seq = get_next_nlmsg_seq();
	//nlh->nlmsg_pid = reqid; //FIXME

	said = (struct xfrm_usersa_id *) mnl_nlmsg_put_extra_header(nlh, sizeof(*said));
	said->spi = htonl(spi);
	said->proto = IPPROTO_ESP;

	said->family = src->sa_family;
	switch (src->sa_family) {
	case AF_INET:
		sin = (const struct sockaddr_in *) src;
		saddr.a4 = sin->sin_addr.s_addr;
		sin = (const struct sockaddr_in *) dst;
		said->daddr.a4 = sin->sin_addr.s_addr;
		break;
	case AF_INET6:
		sin6 = (const struct sockaddr_in6 *) src;
		memcpy(saddr.a6, &sin6->sin6_addr, sizeof(saddr.a6));
		sin6 = (const struct sockaddr_in6 *) dst;
		memcpy(said->daddr.a6, &sin6->sin6_addr, sizeof(said->daddr.a6));
		break;
	default:
		fprintf(stderr, "ERR: unsupported address family %u\n", src->sa_family);
		return -1;
	}

	mnl_attr_put(nlh, XFRMA_SRCADDR, sizeof(saddr), (void *)&saddr);

	rc = transceive_mnl(mnl_sock, nlh, rx_buf, MNL_SOCKET_BUFFER_SIZE, &rx_nlh);
	if (rc < 0) {
		fprintf(stderr, "ERR: cannot delete IPsec SA: %s\n", strerror(errno));
		return -1;
	}

	/* FIXME: parse response */

	return 0;
}

int xfrm_sa_add(struct mnl_socket *mnl_sock, uint32_t reqid,
		const struct sockaddr *src, const struct sockaddr *dst, uint32_t spi,
		const struct xfrm_algo *auth_algo, const struct xfrm_algo *ciph_algo)
{
	uint8_t msg_buf[MNL_SOCKET_BUFFER_SIZE];
	uint8_t rx_buf[MNL_SOCKET_BUFFER_SIZE];
	struct xfrm_usersa_info *sainfo;
	struct nlmsghdr *nlh, *rx_nlh;
	int rc;

	nlh = mnl_nlmsg_put_header(msg_buf);
	nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_CREATE | NLM_F_EXCL | NLM_F_ACK;
	nlh->nlmsg_type = XFRM_MSG_NEWSA;
	nlh->nlmsg_seq = get_next_nlmsg_seq();
	//nlh->nlmsg_pid = reqid; //FIXME

	sainfo = (struct xfrm_usersa_info *) mnl_nlmsg_put_extra_header(nlh, sizeof(*sainfo));
	sainfo->sel.family = src->sa_family;
	rc = sockaddrs2xfrm_sel(&sainfo->sel, src, dst);
	if (rc < 0)
		return -EINVAL;

	sainfo->sel.user = htonl(XFRM_USER_ID);

	sainfo->saddr = sainfo->sel.saddr;
	sainfo->id.daddr = sainfo->sel.daddr;

	sainfo->id.spi = htonl(spi);
	sainfo->id.proto = IPPROTO_ESP;

	sainfo->lft.soft_byte_limit = XFRM_INF;
	sainfo->lft.hard_byte_limit = XFRM_INF;
	sainfo->lft.soft_packet_limit = XFRM_INF;
	sainfo->lft.hard_packet_limit = XFRM_INF;
	sainfo->reqid = reqid;
	sainfo->family = src->sa_family;
	sainfo->mode = XFRM_MODE_TRANSPORT;
	sainfo->replay_window = 32;

	mnl_attr_put(nlh, XFRMA_ALG_AUTH, sizeof(struct xfrm_algo) + auth_algo->alg_key_len, auth_algo);

	mnl_attr_put(nlh, XFRMA_ALG_CRYPT, sizeof(struct xfrm_algo) + ciph_algo->alg_key_len, ciph_algo);

	rc = transceive_mnl(mnl_sock, nlh, rx_buf, MNL_SOCKET_BUFFER_SIZE, &rx_nlh);
	if (rc < 0) {
		fprintf(stderr, "ERR: cannot create IPsec SA: %s\n", strerror(errno));
		return -1;
	}

	/* FIXME: parse response */

	return 0;
}

/***********************************************************************
 * Security Policy
 ***********************************************************************/

int xfrm_policy_add(struct mnl_socket *mnl_sock,
		    const struct sockaddr *src, const struct sockaddr *dst, uint32_t spi, bool dir_in)
{
	uint8_t msg_buf[MNL_SOCKET_BUFFER_SIZE];
	uint8_t rx_buf[MNL_SOCKET_BUFFER_SIZE];
	struct xfrm_userpolicy_info *pinfo;
	struct xfrm_user_tmpl tmpl;
	struct nlmsghdr *nlh, *rx_nlh;
	int rc;

	memset(&tmpl, 0, sizeof(tmpl));

	nlh = mnl_nlmsg_put_header(msg_buf);
	nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_CREATE | NLM_F_EXCL | NLM_F_ACK;
	nlh->nlmsg_type = XFRM_MSG_NEWPOLICY;
	nlh->nlmsg_seq = get_next_nlmsg_seq();
	//nlh->nlmsg_pid = reqid; //FIXME

	pinfo = (struct xfrm_userpolicy_info *) mnl_nlmsg_put_extra_header(nlh, sizeof(*pinfo));

	rc = sockaddrs2xfrm_sel(&pinfo->sel, src, dst);
	if (rc < 0)
		return -EINVAL;

	pinfo->sel.user = htonl(XFRM_USER_ID);

	pinfo->lft.soft_byte_limit = XFRM_INF;
	pinfo->lft.hard_byte_limit = XFRM_INF;
	pinfo->lft.soft_packet_limit = XFRM_INF;
	pinfo->lft.hard_packet_limit = XFRM_INF;
	pinfo->priority = 2342; // FIXME
	pinfo->action = XFRM_POLICY_ALLOW;
	pinfo->share = XFRM_SHARE_ANY;

	if (dir_in)
		pinfo->dir = XFRM_POLICY_IN;
	else
		pinfo->dir = XFRM_POLICY_OUT;

	tmpl.id.proto = IPPROTO_ESP;
	tmpl.id.daddr = pinfo->sel.daddr;
	tmpl.saddr = pinfo->sel.saddr;
	tmpl.family = pinfo->sel.family;
	tmpl.reqid = spi;
	tmpl.mode = XFRM_MODE_TRANSPORT;
	tmpl.aalgos = 0xffffffff;
	tmpl.ealgos = 0xffffffff;
	tmpl.calgos = 0xffffffff;
	mnl_attr_put(nlh, XFRMA_TMPL, sizeof(tmpl), &tmpl);

	rc = transceive_mnl(mnl_sock, nlh, rx_buf, MNL_SOCKET_BUFFER_SIZE, &rx_nlh);
	if (rc < 0) {
		fprintf(stderr, "ERR: cannot create IPsec policy: %s\n", strerror(errno));
		return -1;
	}

	/* FIXME: parse response */

	return 0;
}


int xfrm_policy_del(struct mnl_socket *mnl_sock,
		    const struct sockaddr *src, const struct sockaddr *dst, bool dir_in)
{
	uint8_t msg_buf[MNL_SOCKET_BUFFER_SIZE];
	uint8_t rx_buf[MNL_SOCKET_BUFFER_SIZE];
	struct xfrm_userpolicy_id *pid;
	struct nlmsghdr *nlh, *rx_nlh;
	int rc;

	nlh = mnl_nlmsg_put_header(msg_buf);
	nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK;
	nlh->nlmsg_type = XFRM_MSG_DELPOLICY;
	nlh->nlmsg_seq = get_next_nlmsg_seq();
	//nlh->nlmsg_pid = reqid; //FIXME

	pid = (struct xfrm_userpolicy_id *) mnl_nlmsg_put_extra_header(nlh, sizeof(*pid));

	rc = sockaddrs2xfrm_sel(&pid->sel, src, dst);
	if (rc < 0)
		return -EINVAL;

	pid->sel.user = htonl(XFRM_USER_ID);

	if (dir_in)
		pid->dir = XFRM_POLICY_IN;
	else
		pid->dir = XFRM_POLICY_OUT;

	rc = transceive_mnl(mnl_sock, nlh, rx_buf, MNL_SOCKET_BUFFER_SIZE, &rx_nlh);
	if (rc < 0) {
		fprintf(stderr, "ERR: cannot delete IPsec policy: %s\n", strerror(errno));
		return -1;
	}

	/* FIXME: parse response */

	return 0;
}
