#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    unsigned short server_port = 5002u;
    struct sockaddr_in server_address, client_address;
    struct hostent *host;
    int client_socket_desc;
    if (argc < 3) {
        printf("Usage: ./client <ip> <port>\n");
        return 0;
    }
    if ((client_socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Error socket()\n");
        return -1;
    }
    host = (struct hostent *) gethostbyname(argv[1]);
    server_address.sin_family = AF_INET;
    bcopy(host->h_addr, &server_address.sin_addr, host->h_length);
    server_address.sin_port = htons(atoi(argv[2]));
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = htonl(INADDR_ANY);
    client_address.sin_port = 0;
    if (bind(client_socket_desc, (struct sockaddr *)&client_address, sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "Error bind(): %d - %s\n", errno, strerror(errno));
        return -1;
    }
    int client_address_length = sizeof(client_address);
    if (getsockname(client_socket_desc, (struct sockaddr*)&client_address, &client_address_length)) {
		fprintf(stderr, "Error getsockname(): %d - %s", errno, strerror(errno));
		return -1;
	}
    char buffer[20];
    int id = (argc > 3) ? (atoi(argv[3])) : (0);
    int server_address_len = sizeof(struct sockaddr_in);
    int server_socket_desc;
    if ((connect(client_socket_desc, (struct sockaddr*)&server_address, server_address_len)) == -1) {
        fprintf(stderr, "Error connect(): %d - %s", errno, strerror(errno));
    }
    for (int i = 0; i < 10; ++i) {
        sprintf(buffer, "%d", id);
        sendto(client_socket_desc, buffer, strlen(buffer), 0, (struct sockaddr*)&server_address, server_address_len);
        printf("CLIENT %s:%d Sended message=%s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), buffer);
        fflush(stdout);
        sleep((double)id);
    }
    buffer[0] = 0;
    sendto(client_socket_desc, buffer, 1, 0, (struct sockaddr*)&server_address, server_address_len);
    printf("CLIENT %s:%d Sended message=%s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), buffer);
    fflush(stdout);
    close(client_socket_desc);


    return 0;
}