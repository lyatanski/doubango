#include <stdio.h>
#include "tinynet.h"

int main(void)
{
    tnet_socket_tcp_t *server_socket = NULL, *client_socket = NULL;
    tnet_socket_type_t type = tnet_socket_type_tcp_ipv4;
    struct sockaddr_storage server_addr, client_addr;
    socklen_t addrlen = sizeof(server_addr);
    int test;
    uint16_t server_port = 0;
    int accepted_fd = -1;
    int ret = 1; // Default to failure

    // Create server socket bound to localhost on an ephemeral port
    server_socket = tnet_socket_create("127.0.0.1", 0, type);
    if (!TNET_SOCKET_IS_VALID(server_socket)) {
        fprintf(stderr, "Failed to create server socket\n");
        goto cleanup;
    }

    // Get the port assigned to the server socket
    if (getsockname(server_socket->fd, (struct sockaddr *)&server_addr, &addrlen) == 0) {
        server_port = ntohs(((struct sockaddr_in *)&server_addr)->sin_port);
    } else {
        fprintf(stderr, "Failed to get server socket port\n");
        goto cleanup;
    }

    // Listen for connections
    if (listen(server_socket->fd, 1) != 0) {
        fprintf(stderr, "Failed to listen on server socket\n");
        goto cleanup;
    }

    // Create client socket and connect to server
    client_socket = tnet_socket_create("127.0.0.1", 0, type);
    if (!TNET_SOCKET_IS_VALID(client_socket)) {
        fprintf(stderr, "Failed to create client socket\n");
        goto cleanup;
    }
    tnet_sockaddr_init("127.0.0.1", server_port, type, &client_addr);

    test = tnet_sockfd_connectto(client_socket->fd, &client_addr);
    if (test != 0) {
        fprintf(stderr, "Client failed to connect to server\n");
        goto cleanup;
    }

    // Accept connection on server side
    accepted_fd = accept(server_socket->fd, NULL, NULL);
    if (accepted_fd < 0) {
        fprintf(stderr, "Server failed to accept connection\n");
        goto cleanup;
    } else {
        printf("Local socket connection established successfully!\n");
        ret = 0; // Success
        //closesocket(accepted_fd);
    }

cleanup:
    TSK_OBJECT_SAFE_FREE(server_socket);
    TSK_OBJECT_SAFE_FREE(client_socket);
    return ret;
}
