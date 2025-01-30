/*
 * Simple TCP-server that listen on user-defined socket
 */

#include <iostream>
#include <sys/socket.h>  // socket(...), recv(...)
#include <cstdlib>       // exit(...)
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...), INET_ADDRSTRLEN
#include <arpa/inet.h>   // inet_pton(...)
#include <string>
#include <cstring>       // std::strerror - analogue of std::perror()
#include <chrono>
#include <boost/program_options.hpp>

#include "wrappers.h"

using namespace std;
using namespace std::chrono_literals;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    int listening_fd, connected_fd;
    uint16_t port_listen_to = 0;
    constexpr int LISTENQ_LEN = 10;
    string file_path, message;
    // ===========================================================================

    server_check_arguments(argc, argv, port_listen_to, file_path);

    try {
        message = get_content(file_path);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    // ====== Создаем сокет для приема запросов на соединение (listening socket) =====
    listening_fd = Socket(AF_INET, SOCK_STREAM, 0);

    // Настраиваем структуру адреса сервера без ограничений (принимаем соединение клиента на любом интерфейсе)
    // Константа INADDR_ANY говорит ядру назначить IP-адрес самостоятельно
//    struct sockaddr_in server_address = {
//            .sin_family = AF_INET,
//            .sin_port = htons(port_listen_to),
//            .sin_addr = {INADDR_ANY},
//            .sin_zero = {}
//    };


    // Настраиваем структуру адреса сервера с ограничениями:(принимаем соединение клиента на любом интерфейсе)
    // - принимаем соединение клиента только на интерфейсе "iface_name" (== "lo");
    // - принимаем соединение клиента только на IP-адресе restricted_ip (== "127.0.0.15")
    struct sockaddr_in server_address = {
            .sin_family = AF_INET,
            .sin_port = htons(port_listen_to),
            .sin_addr = {},
            .sin_zero = {}
    };
    const string restricted_ip = "127.0.0.15";
    Inet_pton(AF_INET, restricted_ip.c_str(), &server_address.sin_addr);

    int level = SOL_SOCKET;
    int opt_name = SO_BINDTODEVICE;
    string iface_name = "lo";
    restrict_to_iface(listening_fd,
                      level,
                      opt_name,
                      (void*) iface_name.c_str(),
                      static_cast<socklen_t>(iface_name.size()));

    // Привязываем сокет "listening_fd" к определённому адресу и порту (в данном случае 0.0.0.0:3444)
    Bind(listening_fd, (struct sockaddr*)&server_address, sizeof(server_address));

    // Вызываем "listen" (в коде wrapper'а), чтобы сокет "listening_fd" мог принимать входящие соединения.
    // Сокет "listening_fd" после вызова "listen" будет находиться в состоянии "LISTEN"
    Listen(listening_fd, LISTENQ_LEN);
    cout << "Server (PID = " << getpid() << ") is waiting connection on port " << port_listen_to << "..." << endl;

    // =============== Принятие соединения с клиентом ===============
    // Создаем структуру "sockaddr_in" для хранения информации о клиенте.
    sockaddr_in client_address{};
    socklen_t client_address_len = sizeof(client_address);

//    // TODO: 1) сделать так, чтобы сервер мог обрабатывать множество клиентов, а не только одного (intro/daytimetcpsrv1.c);
//             2) завернуть клиентов и сервер в сеть на основе docker;
//             3) переписать код на идиоматический С++.

    connected_fd = Accept(listening_fd, (struct sockaddr*) &client_address, &client_address_len);

    // =============== Вывод информации о подключении клиента ===============
    char buf[INET_ADDRSTRLEN];
    Inet_ntop(AF_INET, &client_address.sin_addr, buf, sizeof(buf));
    cout << "Connected client ==> " << buf
         << ":" << ntohs(client_address.sin_port) << endl << endl;

    // =============== Отправка данных подключившемуся клиенту ===============
    size_t message_len = message.size();
    size_t total_sent = 0;

    size_t count = 0;
    constexpr size_t buffer_size = 1024;
    char buffer[buffer_size];
    
    while (total_sent < message_len) {
        size_t len_to_send = std::min(buffer_size, message_len - total_sent);
        memcpy(buffer, message.c_str() + total_sent, len_to_send);
        ssize_t num_bytes_sent = Send(connected_fd, buffer, len_to_send, 0);
        ++count;
        total_sent += static_cast<size_t>(num_bytes_sent);
    }

    cout << "Function send was called " << count << " times." << endl;
    cout << "Message sent to client." << endl;

    // Закрываем сокет клиента (connected_fd), т.к. его нужно закрыть сразу после завершения общения с клиентом.
    // При этом listening_fd продолжает слушать входящие соединения.
    Close(connected_fd);

    // Закрытие сокета сервера (listening_fd) происходит после того, как ВСЕ соединения с клиентами завершены,
    Close(listening_fd);

    return 0;
}
