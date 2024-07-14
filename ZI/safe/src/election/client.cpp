#include <Safe.hpp>
#include <string>
#include <MD5.h>
#include <sstream>
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


int main(int argc, char **argv)
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
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t server_address_length = sizeof(server_address);
    if (connect(my_socket, ( struct sockaddr * )&server_address, server_address_length) == -1) {
        printf("Error %d: %s\n", errno, strerror(errno));
    }

    Safe::long_num rnd = Safe::Math::get_num(1, 100000);
    rnd = rnd - (rnd & 0b11);
    std::cout << rnd << " == rnd\n";
    int v = std::stoi(argv[1]);
    Safe::long_num n = rnd | v;
    std::cout << n << " == n\n";

    std::cout << "Vote generated\n";

    Safe::long_num D;
    Safe::long_num N;
    recv(my_socket, &D, sizeof(D), MSG_WAITALL);
    recv(my_socket, &N, sizeof(N), MSG_WAITALL);
    std::cout << D << " == D\t" << N << " == N\n";
    // принять <D, N> от сервера
    std::cout << "<D, N> received\n";
    Safe::long_num r = Safe::Math::get_mutually_prime(N, N);
    std::cout << r << " == r\n";
    auto tmp = std::to_string(n);
    auto str_hash = md5(tmp);
    bigint H(str_hash, 16);
    Safe::long_num h = (H - bigint(N) * (H / bigint(N))).convert_ll();
    std::cout << "str_hash = " << str_hash << std::endl;
    std::cout << h << " == h\n";
    Safe::long_num h_ = Safe::Math::mod((h % N) * Safe::Math::pow_mod(r, D, N), N);
    std::cout << h_ << " == h_\n";
    send(my_socket, &h_, sizeof(h_), MSG_CONFIRM);
    std::cout << "h_ sended\n";
    // отправить h_
    Safe::long_num s_;
    recv(my_socket, &s_, sizeof(s_), MSG_WAITALL);
    std::cout << "s_ received\n";
    std::cout << s_ << " == s_\n";
    // принять s_
    Safe::long_num s = Safe::Math::mod((s_ % N) * Safe::Math::inverse(r, N), N);
    std::cout << s << " == s\n";
    send(my_socket, &n, sizeof(n), MSG_CONFIRM);
    send(my_socket, &s, sizeof(s), MSG_CONFIRM);
    std::cout << "<n, s> sended\n";
    // отправить <n, s>
    close(my_socket);
    return 0;
}