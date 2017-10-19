#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <memory.h>

int server_udp_main() {
    char *address_str = "127.0.0.1";
    int port = 2883;
    int srv_socket = socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in address;
    address.sin_family = PF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(address_str);
    bind(srv_socket, (const struct sockaddr *) &address, sizeof(address));
    struct sockaddr_storage storage;

    socklen_t as = sizeof storage;

    char buffer[32];
    while(1) {
        memset(buffer, 0, 32);
        size_t bytes = (size_t) recvfrom(srv_socket, buffer, 32, 0, (struct sockaddr *) &storage, &as);
        printf("New message received: %s\n", buffer);
        if(buffer[0] == '@')
            return 0;
        for(int i = 0; i < bytes; ++i)
            buffer[i] = (char) toupper(buffer[i]);
        sendto(srv_socket, buffer, bytes, 0, (const struct sockaddr *) &storage, as);
    }
}