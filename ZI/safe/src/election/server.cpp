#include <Safe.hpp>
#include <MD5.h>
#include <sstream>
#include <thread>
#include <bigint/newbigint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include <random>
#include <iostream>
#include <errno.h>
#include <string.h>

void client_handler(int sock)
{
    Safe::Encryption::RSA rsa;
    auto [D, N] = rsa.get_open_key();
    auto C = rsa.get_close_key();
    if (send(sock, &D, sizeof(D), MSG_CONFIRM) == -1)
    {
        printf("Error %d: %s\n", errno, strerror(errno));
    }
    if (send(sock, &N, sizeof(N), MSG_CONFIRM) == -1)
    {
        printf("Error %d: %s\n", errno, strerror(errno));
    }
    std::cout << "<D, N> sended\n";
    std::cout << D << " == D\n";
    std::cout << N << " == N\n";
    // отправить <D, N>
    Safe::long_num h_;
    if (recv(sock, &h_, sizeof(h_), MSG_WAITALL) == -1)
    {
        printf("Error %d: %s\n", errno, strerror(errno));
    }
    std::cout << h_ << " == h_\n";
    // принять h_ и занести в базу бюллетень
    Safe::long_num s_ = Safe::Math::pow_mod(h_, C, N);
    if (send(sock, &s_, sizeof(s_), MSG_CONFIRM) == -1)
    {
        printf("Error %d: %s\n", errno, strerror(errno));
    }
    std::cout << s_ << " == s_\n";
    // отправить s_
    Safe::long_num n;
    Safe::long_num s;
    if (recv(sock, &n, sizeof(n), MSG_WAITALL) == -1)
    {
        printf("Error %d: %s\n", errno, strerror(errno));
    }
    if (recv(sock, &s, sizeof(s), MSG_WAITALL) == -1)
    {
        printf("Error %d: %s\n", errno, strerror(errno));
    }
    std::cout << n << " == n\t" << s << " == s\n";
    // принять <n, s>
    Safe::long_num check = Safe::Math::pow_mod(s, D, N);
    auto str_hash = md5(std::to_string(n));
    bigint big_n(str_hash, 16);
    Safe::long_num h;
    h = (big_n - bigint(N) * (big_n / N)).convert_ll();
    std::cout << h << " == h\n";
        std::cout << check << " == check\n";
    if (h == check)
    {
        std::cout << "Good\n";
        // занести в базу данных голос
    }
    else
    {
        std::cout << "Bad\n";
    }
    close(sock);
}

void server_handler(int my_socket)
{
    char c;
    std::cin >> c;
    close(my_socket);
    exit(0);
}

int main()
{
    const int global_port = 43876;
    int my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (my_socket < 0)
    {
        printf("Socket creation failed: %s\n", strerror(errno));
    }
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(global_port); // Port number
    server_address.sin_addr.s_addr = INADDR_ANY;
    socklen_t server_address_length = sizeof(server_address);
    bool isError = bind(my_socket, (struct sockaddr *)&server_address, server_address_length) == -1;
    if (isError)
    {
        printf("Error %d: %s\n", errno, strerror(errno));
    }
    if (listen(my_socket, 5) == -1)
    {
        printf("Error %d: %s\n", errno, strerror(errno));
        return -1;
    }
    int new_sock = -1;
    std::thread exit_thread(server_handler, my_socket);
    while (true)
    {
        sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);
        new_sock = accept(my_socket, reinterpret_cast<sockaddr*>(&client_address), &client_address_length);
        if (new_sock == -1) {
            printf("Error %d: %s\n", errno, strerror(errno));
            continue;;
        }
        std::cout << "client connected: " << new_sock << std::endl;
        // client_handler(new_sock);
        std::thread t(client_handler, new_sock);
        t.detach();
    }
    printf("Error %d: %s\n", errno, strerror(errno));

    return 0;
}