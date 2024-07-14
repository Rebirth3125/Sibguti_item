#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char** argv) {
    unsigned short server_port = 5002u;
    int server_socket_desc;
    if ((server_socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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
    int nbytes_read = 0;
    char buffer[100];
    int client_address_length = sizeof(struct sockaddr_in);
    if (listen(server_socket_desc, 5) == -1) {
        fprintf(stderr, "Error listen(): %d - %s", errno, strerror(errno));
    }
    printf("SERVER: Listening...\n");
    signal(SIGCHLD, SIG_IGN); 
    pid_t pid;
    int i = 0;
    while (i < 3) { 
        int client_socket_desc = accept(server_socket_desc, (struct sockaddr*)&client_address, &client_address_length);
        switch (pid = fork()) 
        {
        case -1: 
            fprintf(stderr, "Error fork(): %d - %s", errno, strerror(errno));
            break;
        case 0: 
            close(server_socket_desc); 
            while (1) {
                bzero(buffer, nbytes_read);
                nbytes_read = recvfrom(client_socket_desc, buffer, 100, 0, (struct sockaddr*)&client_address, &client_address_length);
                printf("FROM %s:%d: %s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), buffer);
                fflush(stdout);
                if (buffer[0] == 0) {
                    close(client_socket_desc);
                    exit(0); 
                }
            }
            break;
        default: 
            close(client_socket_desc);
            ++i;
            break;
        }

    }
    close(server_socket_desc);

    return 0;
}