/*
 * Simple TCP-server that listen on user-defined socket
 */

#include <iostream>

#include <sys/types.h>   // socket(...)
#include <sys/socket.h>  // socket(...), recv(...)

#include <cstdlib>       // exit(...)
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)
#include <arpa/inet.h>   // inet_pton(...)
#include <cstdio>
#include <unistd.h>      // close(...)

#include <string>
#include <cstring>       // std::strerror - analogue of std::perror()

using namespace std;

int main(int argc, char* argv[]) {
//    if (argc != 2) {
//        cerr << "usage: " << argv[0] << " <port_to_listen_to>" << endl;
//        perror("wrong number of arguments");
//        exit(EXIT_FAILURE);
//    }

//    So first we will create a string to hold the data that we're going to send all the clients
//    const char* server_message = "You have reached the server";

    string server_message = "You have reached the server";
    const int MAX_QUEUE_PENDING_CONNECTIONS_LEN = 10;
    //    const int port_listen_to = htons(static_cast<uint16_t>(stoi(argv[1])));
    const int port_listen_to = 43180;

    // ====== Создаем серверный сокет (Listening Socket) =====
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        cerr << "Socket creation error" << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    // =======================================================

    // ======== Настраиваем структуру адреса сервера =========
    struct sockaddr_in server_address = {
            .sin_family = AF_INET,
            .sin_port = port_listen_to,
            .sin_addr = {INADDR_ANY},
            .sin_zero = {}
    };
    // =======================================================

    // ========== Привязываем сокет к адресу и порту =========
    if (bind(server_sock, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0) {
        cerr << "Socket bind error: " << strerror(errno) << std::endl;
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // =============== Начинаем прослушивание порта ===============
    if ((listen(server_sock, MAX_QUEUE_PENDING_CONNECTIONS_LEN)) < 0) {
        cerr << "Listen error: " << strerror(errno) << endl;
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    cout << "Server is waiting connection on port " << port_listen_to << "..." << endl;

    // =============== Принимаем соединение от клиента ===============
    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    int client_sock = accept(server_sock,
                             reinterpret_cast<sockaddr*>(&client_address),
                             &client_address_len);
    if (client_sock < 0) {
        cerr << "Accept connection error: " << strerror(errno) << endl;
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    cout << "Client is connected!" << endl;

    // =============== Читаем данные от клиента ===============
    string received_data;
    char buffer[1024];
    ssize_t bytes_received;

    while( (bytes_received = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        received_data.append(buffer, bytes_received);
    }

    if (bytes_received < 0) {
        cerr << "Reading data error (recv): " << strerror(errno) << endl;
    } else {
        cout << "Following message is received from client: " << received_data << endl;
    }

    // =============== Отправляем ответ клиенту ===============
    const string response = "Hello, Client!";
    if (send(client_sock, response.c_str(), response.size(), 0) < 0) {
        cerr << "Data sending error: " << strerror(errno) << endl;
    }

    // Закрываем соединение с клиентом
    close(client_sock);

    // Закрываем серверный сокет
    close(server_sock);

    return 0;
}
