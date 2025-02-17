/*
 * Simple ECHO TCP-server
 */

#include <iostream>
#include <sys/socket.h>  // socket(...), recv(...)
#include <cstdlib>       // exit(...)
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...), INET_ADDRSTRLEN
#include <string>
#include <chrono>
#include <unistd.h>      // getpid(), pause()

#include <boost/program_options.hpp>

#include "wrappers.h"
#include "utilities.h"
#include "parsing.h"

using namespace std;
using namespace std::chrono_literals;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    int listen_fd, connected_fd;
    uint16_t port_listen_to = 0;
    constexpr int LISTENQ_LEN = 100;

    echo_server_check_arguments(argc, argv, port_listen_to);
    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address = {
            .sin_family = AF_INET,
            .sin_port = htons(port_listen_to),
            .sin_addr = {INADDR_ANY},
            .sin_zero = {}
    };

    Bind(listen_fd, (struct sockaddr*)&server_address, sizeof(server_address));
    Listen(listen_fd, LISTENQ_LEN);

    cout << "Echo server (PID = " << getpid() << ", listen_fd = " << listen_fd <<
            ") is waiting connection on port " << port_listen_to << "..." << endl;

    // =============== Принятие соединения с клиентом ===============
    // Универсальная структура "sockaddr_storage" нужна для сохранения информации о подключившемся клиенте.
    sockaddr_storage client_address{};  // используем универсальную структуру адреса
    socklen_t client_address_len = sizeof(client_address);

    for (;;) {
        connected_fd = Accept(listen_fd, (struct sockaddr*) &client_address, &client_address_len);
        if (connected_fd < 0) {
            if (errno == EINTR) {
                continue;  // назад в цикл for
            } else {
                cerr << "accept error" << strerror(errno) << endl;
                exit(EXIT_FAILURE);
            }
        }
        // =============== Вывод информации о подключении клиента ===============
        print_info_about_connected_client(client_address);

        // =============== Обработка каждого подсоединившегося клиента в отдельном процессе ===============
        pid_t child_pid = Fork();
        if (child_pid == 0) {
            cout << "=== Child process with PID = " << getpid() << " started processing. ===" << endl;
            Close(listen_fd);
            server_str_echo(connected_fd);
            cout << "=== Child process with PID = " << getpid() << " ENDED processing. ===" << endl;
            cout << "=====================================================================\n" << endl;
            Close(connected_fd);

            exit(0);
        }

        // Родитель закрывает клиентский сокет, т.к. обработка соединения выполняется concurrently дочерним процессом.
         Close(connected_fd);
    }
}