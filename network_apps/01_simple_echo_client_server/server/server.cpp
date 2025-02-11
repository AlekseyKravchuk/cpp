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

using namespace std;
using namespace std::chrono_literals;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    int listen_fd, connected_fd;
    uint16_t port_listen_to = 0;
    constexpr int LISTENQ_LEN = 100;
    // ===========================================================================

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

        // =============== Вывод информации о подключении клиента ===============
        auto [client_ip, client_port] = get_ip_port_from_addr_struct(client_address);
        cout << "Connected client: [IP = " << client_ip << "], "
            << "[PORT = " << client_port << "]" << endl;

        // =============== Обработка подсоединившегося клиента в отдельном процессе ===============
        pid_t pid = fork();
        if (pid == -1) {
            cerr << "!!! fork() не смог создать процесс для обработки входящего соединения !!!" << endl;
        }

        if (pid == 0) {
            // Обработка внутри дочернего процесса

            // Дочерний процесс — приостанавливаемся и ждем отладчика
            cout << "Child process PID: " << getpid() << " (Attach debugger now!)" << endl;
//            pause(); // Остановить процесс, пока мы не подсоединимся отладчиком

            Close(listen_fd);  // Закрываем прослушиваемый сокет в дочернем процессе
            server_str_echo(connected_fd);
            Close(connected_fd);  // обработка клиента завершена => закрываем сокет (connected socket)
            exit(0);
        }
        // ================================= КОНЕЦ обработки =================================
        // Родитель закрывает клиентский сокет, т.к. обработка соединения выполняется concurrently дочерним процессом.
        Close(connected_fd);
    }

    // Закрытие слушающего сокета сервера (listen_fd) происходит после того, как ВСЕ соединения с клиентами завершены,
    Close(listen_fd);

    return 0;
}