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
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    const int MAX_QUEUE_PENDING_CONNECTIONS_LEN = 10;
//    const int port_listen_to = htons(static_cast<uint16_t>(stoi(argv[1])));
    const int port_listen_to = 43180;

    struct sockaddr_in srv_address = {
            .sin_family = AF_INET,
            .sin_port = port_listen_to,
            .sin_addr = {INADDR_ANY},
            .sin_zero = {}
    };

    // =============== Привязываем сокет к адресу и порту ===============
    if (bind(server_socket_fd, reinterpret_cast<sockaddr*>(&srv_address), sizeof(srv_address)) < 0) {
        cerr << "Socket bind error: " << strerror(errno) << std::endl;
        close(server_socket_fd);
        exit(EXIT_FAILURE);
    }

    // =============== Начинаем прослушивание порта ===============
    if ((listen(server_socket_fd, MAX_QUEUE_PENDING_CONNECTIONS_LEN)) < 0) {
        cerr << "Listen error: " << strerror(errno) << endl;
        close(server_socket_fd);
        exit(EXIT_FAILURE);
    }
    cout << "Server is waiting connection on port " << port_listen_to << "..." << endl;

    // =============== Принимаем соединение от клиента ===============
    // TODO:
    int client_socket;
    client_socket = accept(server_socket_fd, nullptr, nullptr);

    // send the message
    send(client_socket, server_message.c_str(), sizeof(server_message.c_str()), 0);

    // close the socket
    close(server_socket_fd);

    return 0;
}
