
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <printf.h>
#include <memory.h>

int client_tcp_main(int args_amount, char *args[]) {
    if(args_amount < 2) {
        printf("Please, enter server address as an argument.");
        return 0;
    }
    char *address = args[1];
    int port = 2882;
    int cl_socket = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = PF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(address);
    if(connect(cl_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("Could not connect to the server");
        return 0;
    }
    char buffer[32];
    while(1) {
        printf("Write something up to 32 symbols:\n");
        memset(buffer, 0, 32);
        scanf("%s", &buffer);
        send(cl_socket, buffer, strlen(buffer), 0);
        if(buffer[0] == '@')
            return 0;
        memset(buffer, 0, 32);
        recv(cl_socket, buffer, 32, 0);
        printf("Server response: %s\n", buffer);
    }
}