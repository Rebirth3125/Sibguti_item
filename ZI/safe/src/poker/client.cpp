#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <Safe.hpp>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <iostream>
#include <errno.h>
#include <string.h>

struct client
{
    sockaddr_in addr;
    socklen_t length;
};

std::string card_to_str(int card_num)
{
    static const std::array<std::string, 4> m = {"♤", "♡", "♧", "♢"};
    static const std::array<std::string, 4> n = {"V", "D", "K", "T"};
    auto tmp = card_num % 13 + 2;
    // ♤♡♧♢
    std::string card_name = "" + m[card_num / 13] + (tmp <= 10 ? std::to_string(tmp) : n[tmp % 10 - 1]);

    return card_name;
}

int main()
{
    srand(time(nullptr));
    const int global_port = 12345;
    const int players_count = 2;
    const int cards_count = 52;
    int myErr = 0;
    auto nextPlayer = [&](int myNum)
    { return (myNum + 1) % players_count; };
    auto prevPlayer = [&](int myNum)
    { return (myNum + players_count - 1) % players_count; };
    struct client player_sockets[players_count];
    int myNum = 0;
    // попытаться создасть сокет с определенным портом
    // если получилось, ожидаем подключения других клиентов
    // если не получилось, значит кто-то уже создал комнату, подключаемся и ожидаем
    int my_socket = socket(AF_INET, SOCK_DGRAM, 0); // udp чтобы клиенты могли между собой общаться
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
    bool isHost = bind(my_socket, (struct sockaddr *)&server_address, server_address_length) == 0;
    if (isHost)
    {
        std::cout << "It is host\n";
        player_sockets[0] = {server_address, server_address_length};
        for (int i = 1; i < players_count; ++i)
        {
            std::cout << "Listen\n";
            // listen(my_socket, players_count-1);
            sockaddr_in client_address;
            memset(&client_address, 0, sizeof(client_address));
            socklen_t client_address_length = sizeof(client_address);
            char tmp;
            recvfrom(my_socket, &tmp, sizeof(tmp), MSG_WAITALL, (struct sockaddr *)&client_address, &client_address_length);
            std::cout << tmp << std::endl;
            // int client_socket = accept(my_socket, (struct sockaddr *)&client_address, &client_address_length);
            std::cout << "Client joined\n";
            player_sockets[i] = {client_address, client_address_length};
        }
        for (int i = 1; i < players_count; ++i)
        {
            std::cout << "Send to client " << i << std::endl;
            myErr = sendto(my_socket, &i, sizeof(myNum), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[i].addr), player_sockets[i].length);
            if (myErr == -1)
            {
                printf("Error %d: %s\n", errno, strerror(errno));
            }
            myErr = sendto(my_socket, &player_sockets, sizeof(player_sockets), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[i].addr), player_sockets[i].length);
            if (myErr == -1)
            {
                printf("Error %d: %s\n", errno, strerror(errno));
            }
            // TODO отправить остальным сокеты и номера всех игроков DONE
        }
    }
    else
    {
        printf("%s\n", strerror(errno));
        std::cout << "It is client\n";
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_address.sin_port = htons(global_port);
        errno = 0;
        // bind(my_socket, (struct sockaddr *)&server_address, server_address_length);
        // printf("%s\n", strerror(errno));
        std::cout << "Connecting\n";
        // connect(my_socket, (struct sockaddr *)&server_address, sizeof(server_address));
        char tmp = 'g';
        sendto(my_socket, &tmp, sizeof(tmp), MSG_CONFIRM, (struct sockaddr *)&server_address, server_address_length);
        std::cout << "Connected\n";
        recvfrom(my_socket, &myNum, sizeof(myNum), MSG_WAITALL, (struct sockaddr *)&server_address, &server_address_length);
        recvfrom(my_socket, &player_sockets, sizeof(player_sockets), MSG_WAITALL, (struct sockaddr *)&server_address, &server_address_length);
        // TODO принять номер и сокеты игроков DONE
    }
    // все игроки подключились
    std::cout << "All players connected\n";
    Safe::long_num p;
    if (isHost)
    {
        Safe::long_num q;
        do
        {
            q = Safe::Math::get_prime(10000);
            p = 2 * q + 1;
        } while (!Safe::Math::is_prime(p));
        // TODO отправить p DONE
        for (int i = 1; i < players_count; ++i)
        {
            if (sendto(my_socket, &p, sizeof(p), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[i].addr), player_sockets[i].length) == -1)
            {
                printf("Error %d: %s\n", errno, strerror(errno));
            }
        }
    }
    else
    {
        if (recvfrom(my_socket, &p, sizeof(p), MSG_WAITALL, (struct sockaddr *)&server_address, &server_address_length) == -1)
        {
            printf("Error %d: %s\n", errno, strerror(errno));
        }
    }
    fflush(stdout);
    Safe::long_num d;
    Safe::long_num c;
    do
    {
        d = Safe::Math::get_num(1 + 1, p - 1 - 1);
        c = Safe::Math::inverse(d, p - 1);
    } while (Safe::Math::mod(c * d, p - 1) != 1);

    std::vector<Safe::long_num> deck;
    std::unordered_map<Safe::long_num, int> accord;
    deck.resize(cards_count); // resize а не reserve потому что потом будет запись по адресу
    if (isHost)
    {
        for (int i = 0; i < cards_count; ++i)
        {
            deck[i] = Safe::Math::get_num(2, p - 1);
        }
        // TODO отправить соответствие числа к карте DONE
        for (int i = 1; i < players_count; ++i)
        {
            sendto(my_socket, deck.data(), deck.size() * sizeof(deck[0]), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[i].addr), player_sockets[i].length);
        }
    }
    else
    {
        // TODO принять соответсвие числа к карте accord DONE
        recvfrom(my_socket, deck.data(), deck.size() * sizeof(deck[0]), MSG_WAITALL, (struct sockaddr *)&server_address, &server_address_length);
    }

    for (int i = 0; i < cards_count; ++i)
    {
        accord[deck[i]] = i;
    }

    // клиент: принять колоду DONE
    if (!isHost)
    {
        recvfrom(my_socket, deck.data(), deck.size() * sizeof(deck[0]), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
    }

    // зашифровать колоду DONE
    for (int i = 0; i < cards_count; ++i)
    {
        auto check = deck[i];
        deck[i] = Safe::Math::pow_mod(deck[i], c, p);
        if (check != Safe::Math::pow_mod(deck[i], d, p))
        {
            printf("TROUBLE: %lld\t%lld\t%lld\n", c, d, p);
        }
    }
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::shuffle(deck.begin(), deck.end(), rng);
    // TODO отправить следующему игроку DONE
    sendto(my_socket, deck.data(), deck.size() * sizeof(deck[0]), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
    recvfrom(my_socket, deck.data(), deck.size() * sizeof(deck[0]), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);

    sendto(my_socket, deck.data(), deck.size() * sizeof(deck[0]), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
    // получить полностью зашифрованную колоду
    // отправить следующему полностью зашифрованную колоду
    // хост: получить копию полностью зашифрованной колоды
    if (isHost)
    {
        recvfrom(my_socket, deck.data(), deck.size() * sizeof(deck[0]), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
    }

    /* РАЗДАЧА */
    // взять шифро-карту

    if (!isHost)
    {
        deck.resize(deck.size() - myNum);
        recvfrom(my_socket, deck.data(), (deck.size()) * sizeof(deck[0]), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
    }

    auto card1 = deck.back();
    deck.pop_back();
    // отправить колоду
    sendto(my_socket, deck.data(), deck.size() * sizeof(deck[0]), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
    // принять колоду
    if (isHost)
    {
        deck.resize(deck.size() - (players_count - 1));
        recvfrom(my_socket, deck.data(), (deck.size()) * sizeof(deck[0]), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
    }

    if (!isHost)
    {
        deck.resize(deck.size() - (players_count - 1));
        recvfrom(my_socket, deck.data(), (deck.size()) * sizeof(deck[0]), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
    }
    // взять шифро-карту
    auto card2 = deck.back();

    deck.pop_back();
    // отправить колоду
    sendto(my_socket, deck.data(), deck.size() * sizeof(deck[0]), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
    if (isHost)
    {
        deck.resize(deck.size() - (players_count - 1));
        recvfrom(my_socket, deck.data(), (deck.size()) * sizeof(deck[0]), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
    }

    /* СТОЛ */
    std::array<Safe::long_num, 5> desk;

    if (isHost)
    {
        // хост: взять 5 карт
        for (int i = 0; i < 5; ++i)
        {
            desk[i] = deck.back();
            deck.pop_back();
        }
        sendto(my_socket, desk.data(), desk.size() * sizeof(desk[0]), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
    }
    else
    {
        // клиент: получить 5 карт
    }
    recvfrom(my_socket, desk.data(), desk.size() * sizeof(desk[0]), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
    if (!isHost)
    {
        sendto(my_socket, desk.data(), desk.size() * sizeof(desk[0]), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
    }
    /*
     // зашифровать i-тым закрытым ключом (i=1,5) **для каждой карты на столе свой ключ
     std::array<std::pair<Safe::long_num, Safe::long_num>, 5> keys;
     for (int i = 0; i < 5; ++i)
     {
         Safe::long_num d = Safe::Math::get_num(1 + 1, p - 1 - 1);
         Safe::long_num c = Safe::Math::inverse(d, p - 1);
         keys[i] = {c, d};
         desk[i] = Safe::Math::pow_mod(desk[i], c, p);
     }
     */
    // отправить следующему
    if (isHost)
    {
        sendto(my_socket, desk.data(), desk.size() * sizeof(desk[0]), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
    }
    recvfrom(my_socket, desk.data(), desk.size() * sizeof(desk[0]), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);

    if (!isHost)
    {
        sendto(my_socket, desk.data(), desk.size() * sizeof(desk[0]), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
    }

    /* ОТКРЫТИЕ РУКИ */
    // отправить шифро-карту
    // принять свою шифро-карту
    // расшифровать карту
    // отправить шифро-карту
    // принять свою шифро-карту
    // расшифровать карту
    for (int i = 0; i < players_count; ++i)
    {
        if (i == myNum)
        {
            sendto(my_socket, &card1, sizeof(card1), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
            recvfrom(my_socket, &card1, sizeof(card1), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
        }
        else
        {
            Safe::long_num tmp;
            recvfrom(my_socket, &tmp, sizeof(tmp), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
            tmp = Safe::Math::pow_mod(tmp, d, p);
            sendto(my_socket, &tmp, sizeof(tmp), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
        }
    }
    card1 = Safe::Math::pow_mod(card1, d, p);

    for (int i = 0; i < players_count; ++i)
    {
        if (i == myNum)
        {

            sendto(my_socket, &card2, sizeof(card2), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
            recvfrom(my_socket, &card2, sizeof(card2), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
        }
        else
        {
            Safe::long_num tmp;
            recvfrom(my_socket, &tmp, sizeof(tmp), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
            tmp = Safe::Math::pow_mod(tmp, d, p);
            sendto(my_socket, &tmp, sizeof(tmp), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
        }
    }
    card2 = Safe::Math::pow_mod(card2, d, p);

    // узнать полученные карты accord[decrypted_num]

    // вывести свои карты
    std::cout << myNum << '\t' << card_to_str(accord[card1]) << std::endl;
    std::cout << myNum << '\t' << card_to_str(accord[card2]) << std::endl;

    /* ОТКРЫТИЕ СТОЛА */
    // каждый отправляет каждому свой i-тый публичный ключ
    // используя публичные ключи всех игроков узнать карту на столе
    for (int i = 0; i < players_count; ++i)
    {
        if (i == myNum)
        {
            sendto(my_socket, &desk, sizeof(desk), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
            recvfrom(my_socket, &desk, sizeof(desk), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
        }
        else
        {
            std::array<Safe::long_num, 5> tmp;
            recvfrom(my_socket, &tmp, sizeof(tmp), MSG_WAITALL, (struct sockaddr *)&(player_sockets[prevPlayer(myNum)].addr), &player_sockets[prevPlayer(myNum)].length);
            for (int j = 0; j < 5; ++j)
            {
                tmp[j] = Safe::Math::pow_mod(tmp[j], d, p);
            }
            sendto(my_socket, &tmp, sizeof(tmp), MSG_CONFIRM, (struct sockaddr *)&(player_sockets[nextPlayer(myNum)].addr), player_sockets[nextPlayer(myNum)].length);
        }
    }
    for (int j = 0; j < 5; ++j)
    {
        desk[j] = Safe::Math::pow_mod(desk[j], d, p);
    }
    // вывести стол
    for (int i = 0; i < 5; ++i)
    {
        std::cout << card_to_str(accord[desk[i]]) << ' ';
    }
    std::cout << std::endl;

    if (isHost)
    {
        // TODO закрыть сокеты
    }
    close(my_socket);
    return 0;
}