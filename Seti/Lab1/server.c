#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    unsigned short server_port = 5002u;
    int server_socket_desc;
    if ((server_socket_desc = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        fprintf(stderr, "Error socket(): %d - %s\n", errno, strerror(errno));
        return -1;
    }
    struct sockaddr_in server_address, client_address;
    bzero((char *)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = 0;
    if (bind(server_socket_desc, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "Error bind(): %d - %s\n", errno, strerror(errno));
        return -1;
    }
    int server_address_length = sizeof(server_address);
    if (getsockname(server_socket_desc, (struct sockaddr*)&server_address, &server_address_length)) {
		fprintf(stderr, "Error getsockname(): %d - %s", errno, strerror(errno));
		return -1;
	}
    printf("SERVER: PORT=%d\n", ntohs(server_address.sin_port));
    fflush(stdout);
    //listen(server_socket_desc, 5);
    int nbytes_read = 0;
    char buffer[100];
    int client_address_length = sizeof(struct sockaddr_in);
    printf("SERVER: Listening...\n");
    while (1) {
        bzero(buffer, nbytes_read);
        nbytes_read = recvfrom(server_socket_desc, buffer, 100, 0, (struct sockaddr*)&client_address, &client_address_length);
        printf("FROM %s:%d: %s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), buffer);
        fflush(stdout);
        if (buffer[0] == 0) {
            break;
        }
    }
    close(server_socket_desc);

    return 0;
}