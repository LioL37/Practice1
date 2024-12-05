#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;

    // Создаем сокет
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Failed to create socket" << endl;
        return EXIT_FAILURE;
    }

    // Настраиваем адрес сервера
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(7432);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    // Пытаемся подключиться к серверу
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Failed to connect to server" << endl;
        close(clientSocket);
        return EXIT_FAILURE;
    }

    cout << "Connected to server" << endl;

    // Закрываем сокет
    close(clientSocket);

    return 0;
}