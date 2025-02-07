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
    int listen_fd, connected_fd;
    uint16_t port_listen_to = 0;
    constexpr int LISTENQ_LEN = 100;
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
    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

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
//    restrict_to_iface(listen_fd,
//                      level,
//                      opt_name,
//                      (void*) iface_name.c_str(),
//                      static_cast<socklen_t>(iface_name.size()));

    // Привязываем сокет "listen_fd" к определённому адресу и порту (в данном случае 127.0.0.15:3444)
    Bind(listen_fd, (struct sockaddr*)&server_address, sizeof(server_address));

    // Вызываем "listen" (в коде wrapper'а), чтобы сокет "listen_fd" мог принимать входящие соединения.
    // Сокет "listen_fd" после вызова "listen" будет находиться в состоянии "LISTEN"
    Listen(listen_fd, LISTENQ_LEN);
    cout << "Server (PID = " << getpid() << ", listen_fd = " << listen_fd <<
            ") is waiting connection on port " << port_listen_to << "..." << endl;

    // =============== Принятие соединения с клиентом ===============
    // Универсальная структура "sockaddr_storage" нужна для сохранения информации о подключившемся клиенте.
    sockaddr_storage client_address{};  // используем универсальную структуру адреса
    socklen_t client_address_len = sizeof(client_address);

//    // TODO: 1) сделать так, чтобы сервер мог обрабатывать множество клиентов, а не только одного (intro/daytimetcpsrv1.c);
//             2) завернуть клиентов и сервер в сеть на основе docker;
//             3) переписать код на идиоматический С++.

    constexpr uint32_t max_clients_count = 3;

    for (uint32_t i = 0; i < max_clients_count; ++i) {
        connected_fd = Accept(listen_fd, (struct sockaddr*) &client_address, &client_address_len);

        // =============== Вывод информации о подключении клиента ===============
        auto [client_ip, client_port] = get_ip_port_from_addr_struct(client_address);
        cout << "Connected client: [IP = " << client_ip << "], "
            << "[PORT = " << client_port << "]" << endl;

        // =============== Обработка подсоединившегося клиента в отдельном процессе ===============
//        usleep(100'000); // sleep for 0.1 second
        pid_t pid = fork();

        if (pid == 0) {
            cout << "Внутри порожденного (дочернего) процесса, pid = " << pid << endl;

            usleep(60'000'000);  // sleep for 120 seconds
            Close(listen_fd);  // Закрываем прослушиваемый сокет в дочернем процессе - здесь он не нужен
            usleep(60'000'000);  // sleep for 120 seconds

            handle_client(connected_fd, message);

            // Закрывать явно клиентский сокет не обязательно, т.к. прекращение процесса (по exit()), в частности,
            // подразумевает закрытие ядром всех открытых дескрипторов.
            Close(connected_fd);  // обработка клиента завершена => закрываем сокет (connected socket)
            exit(0);
        }
        // ================================= КОНЕЦ обработки =================================
        // Процесс-родитель закрывает клиентский сокет,
        // т.к. обработка соединения выполняется параллельно (concurrently) дочерним процессом.
        Close(connected_fd);

//        // итеративное обслуживание клиентов
//        handle_client(connected_fd, message);

        // Чтобы сервер корректно завершил соединение, нужно явно отправить FIN перед вызовом Close(connected_fd)
        // Используем функцию shutdown() для корректного завершения передачи данных/
        // Это обеспечит корректное завершение соединения с передачей данных и с использованием флага FIN.
//         shutdown(connected_fd, SHUT_RDWR);  // Ожидаем завершение передачи данных обеими сторонами

        // Закрываем сокет клиента (connected_fd), т.к. его нужно закрыть сразу после завершения общения с клиентом.
        // При этом:
        // 1) listen_fd продолжает слушать входящие соединения.
        // 2) это не освобождает ресурсы на стороне клиента. На клиенте нужно явно вызвать close().
        // Close(connected_fd);
    }

    // Закрытие сокета сервера (listen_fd) происходит после того, как ВСЕ соединения с клиентами завершены,
    Close(listen_fd);

    return 0;
}