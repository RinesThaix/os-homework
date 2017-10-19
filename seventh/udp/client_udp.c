
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <printf.h>
#include <memory.h>

int client_udp_main(int args_amount, char *args[]) {
    if(args_amount < 2) {
        printf("Please, enter server address as an argument.");
        return 0;
    }
    char *address = args[1];
    int port = 2883;
    int cl_socket = socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = PF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(address);
    char buffer[32];
    socklen_t ssa = sizeof server_address;
    while(1) {
        printf("Write something up to 32 symbols:\n");
        memset(buffer, 0, 32);
        scanf("%s", &buffer);
        sendto(cl_socket, buffer, strlen(buffer), 0, (const struct sockaddr *) &server_address, sizeof(server_address));
        if(buffer[0] == '@')
            return 0;
        memset(buffer, 0, 32);
        recvfrom(cl_socket, buffer, 32, 0, (struct sockaddr *) &server_address, &ssa);
        printf("Server response: %s\n", buffer);
    }
}