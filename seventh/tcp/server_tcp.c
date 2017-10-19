#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <memory.h>

void *cl_processor(void *args) {
    int *iargs = (int *) args;
    int id = iargs[0], cl_socket = iargs[1];
    printf("New client (id %d) connected!\n", id); //use id for anything else?
    int bytes = 1;
    char buffer[32];
    while(bytes > 0) {
        memset(buffer, 0, 32);
        bytes = recv(cl_socket, buffer, 32, 0);
        printf("Message from client %d: %s\n", id, buffer);
        if(buffer[0] == '@')
            exit(0);
        for(int i = 0; i < bytes; ++i)
            buffer[i] = (char) toupper(buffer[i]);
        send(cl_socket, buffer, bytes, 0);
    }
}

int server_tcp_main() {
    char *address_str = "127.0.0.1";
    int port = 2882;
    int srv_socket = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = PF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(address_str);
    bind(srv_socket, (const struct sockaddr *) &address, sizeof(address));
    struct sockaddr_storage storage;

    socklen_t as = sizeof storage;

    int cl_id = 0;
    while(1) {
        int cl_socket = accept(srv_socket, (struct sockaddr *) &storage, &as);
        pthread_t cl_thread;
        int *args = malloc(sizeof(int) << 1);
        args[0] = ++cl_id;
        args[1] = cl_socket;
        pthread_create(&cl_thread, NULL, &cl_processor, args);
    }
}