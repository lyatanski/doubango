/* Demo/Debug tool for Linux kernel IPsec interfacing via netlink XFRM
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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define _GNU_SOURCE
#include <getopt.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "netlink_xfrm.h"

static struct mnl_socket *g_mnl_socket;

static int gai_helper(struct sockaddr_storage *out, const char *node, const char *port)
{
	struct addrinfo hints = {
		.ai_flags = AI_NUMERICSERV | AI_NUMERICHOST,
	};
	struct addrinfo *res;
	int rc;

	rc = getaddrinfo(node, port, &hints, &res);
	if (rc != 0) {
		fprintf(stderr, "getaddrinfo(%s): %s\n", node, gai_strerror(rc));
		return -1;
	}

	memcpy(out, res->ai_addr, res->ai_addrlen);

	freeaddrinfo(res);

	return 0;
}

static int cmd_alloc_spi(int argc, char **argv)
{
	char *src_ip_str = NULL;
	char *dst_ip_str = NULL;
	struct sockaddr_storage src_addr, dst_addr;
	uint32_t spi_out;
	int rc;

	while (1) {
		int option_index = 0, c;
		static const struct option long_options[] = {
			{"help", 0, 0, 'h'},
			{"src-ip", 1, 0, 's'},
			{"dst-ip", 1, 0, 'd'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "hs:d:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			break;
		case 's':
			src_ip_str = optarg;
			break;
		case 'd':
			dst_ip_str = optarg;
			break;
		}
	}

	if (!src_ip_str || !dst_ip_str) {
		fprintf(stderr, "Both src and dst IP must be provided\n");
		exit(1);
	}

	gai_helper(&src_addr, src_ip_str, NULL);
	gai_helper(&dst_addr, dst_ip_str, NULL);

	rc = xfrm_spi_alloc(g_mnl_socket, 2342, &spi_out, (const struct sockaddr *)&src_addr, (const struct sockaddr *)&dst_addr);
	if (rc < 0) {
		fprintf(stderr, "Error allocating SPI: %s\n", strerror(errno));
		exit(1);
	}
	printf("Allocated SPI 0x%08x\n", spi_out);

	return 0;
}

static int cmd_sa_add(int argc, char **argv)
{
	char *src_ip_str = NULL;
	char *dst_ip_str = NULL;
	char *sport = NULL;
	char *dport = NULL;
	char *ciph_alg_str = NULL;
	char *auth_alg_str = NULL;
	struct sockaddr_storage src_addr, dst_addr;
	struct xfrm_algobuf auth_algo, ciph_algo;
	uint32_t spi = 0;
	int rc;

	memset(&auth_algo, 0, sizeof(auth_algo));
	memset(&ciph_algo, 0, sizeof(ciph_algo));

	while (1) {
		int option_index = 0, c;
		static const struct option long_options[] = {
			{"help", 0, 0, 'h'},
			{"src-ip", 1, 0, 's'},
			{"dst-ip", 1, 0, 'd'},
			{"src-port", 1, 0, 'S'},
			{"dst-port", 1, 0, 'D'},
			{"spi", 1, 0, 'p'},
			{"auth-alg", 1, 0, 'a'},
			{"ciph-alg", 1, 0, 'c'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "hs:d:S:D:p:a:c:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			break;
		case 's':
			src_ip_str = optarg;
			break;
		case 'd':
			dst_ip_str = optarg;
			break;
		case 'S':
			sport = optarg;
			break;
		case 'D':
			dport = optarg;
			break;
		case 'p':
			spi = atoi(optarg);
			break;
		case 'a':
			auth_alg_str = optarg;
			break;
		case 'c':
			ciph_alg_str = optarg;
			break;
		}
	}

	if (!src_ip_str || !dst_ip_str) {
		fprintf(stderr, "Both src and dst IP must be provided\n");
		exit(1);
	}

	if (!sport || !dport) {
		fprintf(stderr, "Both src and dst port must be provided\n");
		exit(1);
	}

	if (!auth_alg_str) {
		fprintf(stderr, "Auth alg must be provided\n");
		exit(1);
	}

	if (!ciph_alg_str)
		ciph_alg_str = "cipher_null";

	strcpy(auth_algo.algo.alg_name, auth_alg_str);
	/* FIXME: key len/data */

	strcpy(ciph_algo.algo.alg_name, ciph_alg_str);
	/* FIXME: key len/data */

	gai_helper(&src_addr, src_ip_str, sport);
	gai_helper(&dst_addr, dst_ip_str, dport);

	rc = xfrm_sa_add(g_mnl_socket, 2325, (struct sockaddr *)&src_addr, (struct sockaddr *)&dst_addr,
			 spi, &auth_algo.algo, &ciph_algo.algo);
	if (rc < 0) {
		fprintf(stderr, "Error adding SA: %s\n", strerror(errno));
		exit(1);
	}

	return 0;
}

static int cmd_sa_del(int argc, char **argv)
{
	char *src_ip_str = NULL;
	char *dst_ip_str = NULL;
	char *sport = NULL;
	char *dport = NULL;
	struct sockaddr_storage src_addr, dst_addr;
	uint32_t spi = 0;
	int rc;

	while (1) {
		int option_index = 0, c;
		static const struct option long_options[] = {
			{"help", 0, 0, 'h'},
			{"src-ip", 1, 0, 's'},
			{"dst-ip", 1, 0, 'd'},
			{"src-port", 1, 0, 'S'},
			{"dst-port", 1, 0, 'D'},
			{"spi", 1, 0, 'p'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "hs:d:S:D:p:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			break;
		case 's':
			src_ip_str = optarg;
			break;
		case 'd':
			dst_ip_str = optarg;
			break;
		case 'S':
			sport = optarg;
			break;
		case 'D':
			dport = optarg;
			break;
		case 'p':
			spi = atoi(optarg);
			break;
		}
	}

	if (!src_ip_str || !dst_ip_str) {
		fprintf(stderr, "Both src and dst IP must be provided\n");
		exit(1);
	}

	if (!sport || !dport) {
		fprintf(stderr, "Both src and dst port must be provided\n");
		exit(1);
	}

	gai_helper(&src_addr, src_ip_str, sport);
	gai_helper(&dst_addr, dst_ip_str, dport);

	rc = xfrm_sa_del(g_mnl_socket, (struct sockaddr *)&src_addr, (struct sockaddr *)&dst_addr, spi);
	if (rc < 0) {
		fprintf(stderr, "Error deleting SA: %s\n", strerror(errno));
		exit(1);
	}

	return 0;
}


static int cmd_policy_add(int argc, char **argv)
{
	char *src_ip_str = NULL;
	char *dst_ip_str = NULL;
	char *sport = NULL;
	char *dport = NULL;
	struct sockaddr_storage src_addr, dst_addr;
	uint32_t spi = 0;
	int rc;

	while (1) {
		int option_index = 0, c;
		static const struct option long_options[] = {
			{"help", 0, 0, 'h'},
			{"src-ip", 1, 0, 's'},
			{"dst-ip", 1, 0, 'd'},
			{"src-port", 1, 0, 'S'},
			{"dst-port", 1, 0, 'D'},
			{"spi", 1, 0, 'p'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "hs:d:S:D:p:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			break;
		case 's':
			src_ip_str = optarg;
			break;
		case 'd':
			dst_ip_str = optarg;
			break;
		case 'S':
			sport = optarg;
			break;
		case 'D':
			dport = optarg;
			break;
		case 'p':
			spi = atoi(optarg);
			break;
		}
	}

	if (!src_ip_str || !dst_ip_str) {
		fprintf(stderr, "Both src and dst IP must be provided\n");
		exit(1);
	}

	if (!sport || !dport) {
		fprintf(stderr, "Both src and dst port must be provided\n");
		exit(1);
	}

	gai_helper(&src_addr, src_ip_str, sport);
	gai_helper(&dst_addr, dst_ip_str, dport);

	rc = xfrm_policy_add(g_mnl_socket, (struct sockaddr *)&src_addr, (struct sockaddr *)&dst_addr,
			 spi, true);
	if (rc < 0) {
		fprintf(stderr, "Error adding SA: %s\n", strerror(errno));
		exit(1);
	}

	return 0;
}

static int cmd_policy_del(int argc, char **argv)
{
	char *src_ip_str = NULL;
	char *dst_ip_str = NULL;
	char *sport = NULL;
	char *dport = NULL;
	struct sockaddr_storage src_addr, dst_addr;
	int rc;

	while (1) {
		int option_index = 0, c;
		static const struct option long_options[] = {
			{"help", 0, 0, 'h'},
			{"src-ip", 1, 0, 's'},
			{"dst-ip", 1, 0, 'd'},
			{"src-port", 1, 0, 'S'},
			{"dst-port", 1, 0, 'D'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "hs:d:S:D:p:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			break;
		case 's':
			src_ip_str = optarg;
			break;
		case 'd':
			dst_ip_str = optarg;
			break;
		case 'S':
			sport = optarg;
			break;
		case 'D':
			dport = optarg;
			break;
		}
	}

	if (!src_ip_str || !dst_ip_str) {
		fprintf(stderr, "Both src and dst IP must be provided\n");
		exit(1);
	}

	if (!sport || !dport) {
		fprintf(stderr, "Both src and dst port must be provided\n");
		exit(1);
	}

	gai_helper(&src_addr, src_ip_str, sport);
	gai_helper(&dst_addr, dst_ip_str, dport);

	rc = xfrm_policy_del(g_mnl_socket, (struct sockaddr *)&src_addr, (struct sockaddr *)&dst_addr,
			 true);
	if (rc < 0) {
		fprintf(stderr, "Error adding SA: %s\n", strerror(errno));
		exit(1);
	}

	return 0;
}


int main(int argc, char **argv)
{
	const char *cmd;

	if (argc <= 1) {
		fprintf(stderr, "Missing first argument (command)\n");
		exit(1);
	}
	cmd = argv[1];

	g_mnl_socket = xfrm_init_mnl_socket();

	if (!strcmp(cmd, "spi-alloc"))
		cmd_alloc_spi(argc-1, argv+1);
	else if (!strcmp(cmd, "sa-add"))
		cmd_sa_add(argc-1, argv+1);
	else if (!strcmp(cmd, "sa-del"))
		cmd_sa_del(argc-1, argv+1);
	else if (!strcmp(cmd, "policy-add"))
		cmd_policy_add(argc-1, argv+1);
	else if (!strcmp(cmd, "policy-del"))
		cmd_policy_del(argc-1, argv+1);
	else {
		fprintf(stderr, "Invalid first argument (command)\n");
		exit(1);
	}

}
