#include "tinynet.h"

#define LOCAL_PORT 5060

static int callback(const tnet_transport_event_t* e) {
    if(e->type == event_data && e->data) {
        printf("Received message: %.*s\n", (int)e->size, (char*)e->data);
    }
    return 0;
}

int run_udp_client() {
    tnet_fd_t fd;
    tnet_transport_t *transport = tnet_transport_create(NULL, 0, tnet_socket_type_udp_ipv4, "UDP CLIENT");
    if (!transport) {
        printf("Failed to create client transport\n");
        return -1;
    }
    tnet_transport_start(transport);

    fd = tnet_transport_connectto_2(transport, "127.0.0.1", LOCAL_PORT);
    if (fd == TNET_INVALID_FD) {
        printf("Failed to connect to server\n");
        TSK_OBJECT_SAFE_FREE(transport);
        return -2;
    }

    const char *message = "Hello from client!";
    tnet_transport_send(transport, fd, message, strlen(message));
    printf("Sent: %s\n", message);

    tsk_thread_sleep(1000); // Give time for message to arrive
    tnet_transport_shutdown(transport);
    TSK_OBJECT_SAFE_FREE(transport);
    return 0;
}

int main() {
    // Start server
    tnet_transport_t *transport = tnet_transport_create("127.0.0.1", LOCAL_PORT, tnet_socket_type_udp_ipv4, "UDP SERVER");
    if (!transport) {
        printf("Failed to create transport\n");
        return -1;
    }
    tnet_transport_set_callback(transport, callback, NULL);
    tnet_transport_start(transport);

    // Server runs in background, waiting for messages
    printf("UDP server running on 127.0.0.1:%d\n", LOCAL_PORT);
    tsk_thread_sleep(500); // Give server time to start

    // Run client in main thread
    run_udp_client();

    printf("Test completed.\n");
    return 0;
}
