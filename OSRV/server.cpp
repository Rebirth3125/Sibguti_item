#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

const int PORT = 12345;
const int BACKLOG = 5;

int fuelLevel = 1000;  

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Ошибка при создании сокета." << endl;
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "Ошибка при привязке сокета к порту." << endl;
        return 1;
    }

    if (listen(serverSocket, BACKLOG) == -1) {
        cerr << "Ошибка при установлении прослушивания." << endl;
        return 1;
    }

    cout << "Сервер хранилища запущен и ожидает клиентов." << endl;

    while (true) {
        sockaddr_in clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
        if (clientSocket == -1) {
            cerr << "Ошибка при принятии клиента." << endl;
            return 1;
        }

        char request[1024];
        recv(clientSocket, request, sizeof(request), 0);

        string response;

        if (strcmp(request, "TRANSFER_FUEL") == 0) {

            int fuelAmount;
            recv(clientSocket, &fuelAmount, sizeof(fuelAmount), 0);

            if (fuelAmount <= fuelLevel) {
                fuelLevel -= fuelAmount;
                response = "FUEL_TRANSFERRED";
            } else {
                response = "INSUFFICIENT_FUEL";
            }
        } else if (strcmp(request, "GET_FUEL_LEVEL") == 0) {

            response = to_string(fuelLevel);
        } else {
            response = "UNKNOWN_REQUEST";
        }

        send(clientSocket, response.c_str(), response.length(), 0);

        close(clientSocket);
    }

    close(serverSocket);

    return 0;
}