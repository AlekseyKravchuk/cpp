/*
 * Simple TCP-server that listen on user-defined socket
 */

#include <iostream>
#include <sys/socket.h>  // socket(...), recv(...)
#include <cstdlib>       // exit(...)
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...), INET_ADDRSTRLEN
#include <string>
#include <cstring>       // std::strerror - analogue of std::perror()
#include <chrono>
#include <boost/program_options.hpp>

#include "wrappers.h"
#include "utilities.h"

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
    struct sockaddr_in server_address = {
            .sin_family = AF_INET,
            .sin_port = htons(port_listen_to),
            .sin_addr = {INADDR_ANY},
            .sin_zero = {}
    };

//    // Настраиваем структуру адреса сервера с ограничениями:
//    // - принимаем соединение клиента только на интерфейсе "iface_name" (== "lo");
//    // - принимаем соединение клиента только на IP-адресе restricted_ip (== "127.0.0.15")
//    struct sockaddr_in server_address = {
//            .sin_family = AF_INET,
//            .sin_port = htons(port_listen_to),
//            .sin_addr = {},
//            .sin_zero = {}
//    };
//    const string restricted_ip = "127.0.0.15";
//    Inet_pton(AF_INET, restricted_ip.c_str(), &server_address.sin_addr);
//
//    int level = SOL_SOCKET;
//    int opt_name = SO_BINDTODEVICE;
//    string iface_name = "lo";
//    restrict_to_iface(listening_fd,
//                      level,
//                      opt_name,
//                      (void*) iface_name.c_str(),
//                      static_cast<socklen_t>(iface_name.size()));

    // Привязываем сокет "listening_fd" к определённому адресу и порту (в данном случае 127.0.0.15:3444)
    Bind(listening_fd, (struct sockaddr*)&server_address, sizeof(server_address));

    // Вызываем "listen" (в коде wrapper'а), чтобы сокет "listening_fd" мог принимать входящие соединения.
    // Сокет "listening_fd" после вызова "listen" будет находиться в состоянии "LISTEN"
    Listen(listening_fd, LISTENQ_LEN);
    cout << "Server (PID = " << getpid() << ") is waiting connection on port " << port_listen_to << "..." << endl;

    // =============== Принятие соединения с клиентом ===============
    // Универсальная структура "sockaddr_storage" нужна для сохранения информации о подключившемся клиенте.
    sockaddr_storage client_address{};  // используем универсальную структуру адреса, позволяющую
    socklen_t client_address_len = sizeof(client_address);

//    // TODO: 1) сделать так, чтобы сервер мог обрабатывать множество клиентов, а не только одного (intro/daytimetcpsrv1.c);
//             2) завернуть клиентов и сервер в сеть на основе docker;
//             3) переписать код на идиоматический С++.

    constexpr uint32_t max_clients_count = 3;

    for (uint32_t i = 0; i < max_clients_count; ++i) {
        connected_fd = Accept(listening_fd, (struct sockaddr*) &client_address, &client_address_len);

        // =============== Вывод информации о подключении клиента ===============
        cout << info_about_connected_client(client_address) << endl;

        // =============== Отправка данных подключившемуся клиенту ===============
        size_t msg_len = message.size();
        size_t total_sent = 0;

        size_t count = 0;
        constexpr size_t buffer_size = 1024;
        char buffer[buffer_size];

        while (total_sent < message.size()) {
            size_t len_to_send = std::min(buffer_size, msg_len - total_sent);
            memcpy(buffer, message.c_str() + total_sent, len_to_send);
            ssize_t num_bytes_sent = Send(connected_fd, buffer, len_to_send, 0);
            ++count;
            total_sent += static_cast<size_t>(num_bytes_sent);
        }

        cout << "Function send was called " << count << " times." << endl;
        cout << "Message sent to client." << endl;

        // Чтобы сервер корректно завершил соединение, нужно явно отправить FIN перед вызовом Close(connected_fd)
        // Используем функцию shutdown() для корректного завершения передачи данных/
        // Это обеспечит корректное завершение соединения с передачей данных и с использованием флага FIN.
        shutdown(connected_fd, SHUT_RDWR);  // Ожидаем завершение передачи данных обеими сторонами

        // Закрываем сокет клиента (connected_fd), т.к. его нужно закрыть сразу после завершения общения с клиентом.
        // При этом:
        // 1) listening_fd продолжает слушать входящие соединения.
        // 2) это не освобождает ресурсы на стороне клиента. На клиенте нужно явно вызвать close().
        Close(connected_fd);
    }

    // Закрытие сокета сервера (listening_fd) происходит после того, как ВСЕ соединения с клиентами завершены,
    Close(listening_fd);

    return 0;
}
