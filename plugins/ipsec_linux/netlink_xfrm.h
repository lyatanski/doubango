#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <linux/xfrm.h>

struct mnl_socket;

struct xfrm_algobuf {
	struct xfrm_algo algo;
	uint8_t buf[sizeof(struct xfrm_algo) + 128];
};

struct mnl_socket *xfrm_init_mnl_socket(void);

int xfrm_spi_alloc(struct mnl_socket *mnl_sock, uint32_t reqid, uint32_t *spi_out,
		   const struct sockaddr *src, const struct sockaddr *dst);

int xfrm_sa_add(struct mnl_socket *mnl_sock, uint32_t reqid,
		const struct sockaddr *src, const struct sockaddr *dst, uint32_t spi,
		const struct xfrm_algo *auth_algo, const struct xfrm_algo *ciph_algo);

int xfrm_sa_del(struct mnl_socket *mnl_sock,
		const struct sockaddr *src, const struct sockaddr *dst, uint32_t spi);

int xfrm_policy_add(struct mnl_socket *mnl_sock,
		    const struct sockaddr *src, const struct sockaddr *dst, uint32_t spi, bool dir_in);

int xfrm_policy_del(struct mnl_socket *mnl_sock,
		    const struct sockaddr *src, const struct sockaddr *dst, bool dir_in);
