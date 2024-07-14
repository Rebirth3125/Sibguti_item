#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 12345;

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Ошибка при создании сокета." << endl;
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "Ошибка при подключении к серверу." << endl;
        return 1;
    }

    // Логика для клиента-транспорта здесь

    close(clientSocket);

    return 0;
}