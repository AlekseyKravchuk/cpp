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
    //    const int port_listen_to = htons(static_cast<uint16_t>(stoi(argv[1])));
    uint16_t port_listen_to = 43180;

    // =============
    if (argc == 1) {
        cout << "parameterless usage:  " << argv[0] << R"( <port_listen_to=43180>)" << endl;
    } else if (argc == 2) {
        port_listen_to = static_cast<uint16_t>(std::stoul(argv[1]));
    } else {
        cerr << "usage: " << argv[0] << " <port_listen_to>" << endl << "OR\n"
             << "usage: " << argv[0] << endl;
        perror("wrong number of arguments");
        exit(EXIT_FAILURE);
    }

    // Сообщение, которое будет передаваться всем клиентам при их подключении к серверу
    string server_message = "You have reached the server";
    const int MAX_QUEUE_PENDING_CONNECTIONS_LEN = 10;

    // ====== Создаем серверный сокет (Listening Socket) =====
    int listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket < 0) {
        cerr << "Socket creation error" << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    // ======== Настраиваем структуру адреса сервера =========
    // AF_INET:   for IPv4
    // AF_UNSPEC: for IPv4 or IPv6
    struct sockaddr_in server_address = {
            .sin_family = AF_UNSPEC,
            .sin_port = htons(port_listen_to),
            .sin_addr = {INADDR_ANY},
            .sin_zero = {}
    };

    // ========== Привязываем сокет к адресу и порту =========
    if (bind(listening_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0) {
        cerr << "Socket bind error: " << strerror(errno) << std::endl;
        close(listening_socket);
        exit(EXIT_FAILURE);
    }

    // =============== Начинаем прослушивание порта ===============
    if ((listen(listening_socket, MAX_QUEUE_PENDING_CONNECTIONS_LEN)) < 0) {
        cerr << "Listen error: " << strerror(errno) << endl;
        close(listening_socket);
        exit(EXIT_FAILURE);
    }
    cout << "Server is waiting connection on port " << port_listen_to << "..." << endl;

    // =============== Принимаем соединение от клиента ===============
    sockaddr_in client_address{};
    socklen_t client_address_len = sizeof(client_address);

    int client_socket = accept(listening_socket,
                               reinterpret_cast<sockaddr*>(&client_address),
                               &client_address_len);
    if (client_socket < 0) {
        cerr << "Accept connection error: " << strerror(errno) << endl;
        close(listening_socket);
        exit(EXIT_FAILURE);
    }
    cout << "Client is connected!" << endl;

    // =============== Читаем данные от клиента ===============
    string received_data;
    char buffer[1024];
    ssize_t bytes_received;

    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received) {
        received_data.append(buffer, static_cast<size_t>(bytes_received));
    }

    while((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        received_data.append(buffer, static_cast<size_t>(bytes_received));
    }

    if (bytes_received < 0) {
        cerr << "Reading data error (recv): " << strerror(errno) << endl;
    } else {
        cout << "Following message is received from client: " << received_data << endl;
    }

    // =============== Отправляем ответ клиенту ===============
    const string response = "Hello, Client!";
    if (send(client_socket, response.c_str(), response.size(), 0) < 0) {
        cerr << "Data sending error: " << strerror(errno) << endl;
    }

    // Закрываем соединение с клиентом
    close(client_socket);

    // Закрываем серверный сокет
    close(listening_socket);

    return 0;
}
