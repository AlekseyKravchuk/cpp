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

#include "wrappers.h"
#include "utilities.h"
#include "parsing.h"

using namespace std;

int main(int argc, char* argv[]) {
    int listen_fd, connected_fd;
    uint16_t port_listen_to = 0;
    constexpr int LISTENQ_LEN = 100;

    http_server_check_arguments(argc, argv, port_listen_to);

    // ============== HTTP-related stuff ==============
    // 1. Открываем файл и читаем его содержимое в std::string
    string html_file_path = "/home/kav/opt/cpp/network_apps/02_http_server/index.html";
    std::ifstream file(html_file_path);
    if (!file) {
        std::cerr << "Ошибка: не удалось открыть файл: " << html_file_path << endl;
        return 1;
    }
    string http_body((istreambuf_iterator<char>(file)),
                      istreambuf_iterator<char>(    ));
    file.close(); // Закрываем файл

    string http_header = "HTTP/1.1 200 OK\\r\\n\\n";
    string http_data = http_header + http_body;
    // ================================================

    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address = {
            .sin_family = AF_INET,
            .sin_port = htons(port_listen_to),
            .sin_addr = {INADDR_ANY},
            .sin_zero = {}
    };

    const string restricted_ip = "127.0.0.15";
    Inet_pton(AF_INET, restricted_ip.c_str(), &server_address.sin_addr);

    int level = SOL_SOCKET;
    int optname = SO_BINDTODEVICE;
    string iface_name = "lo";
    restrict_to_iface(listen_fd, level, optname, (void*) iface_name.c_str(), static_cast<socklen_t>(iface_name.size()));

    Bind(listen_fd, (struct sockaddr*)&server_address, sizeof(server_address));
    Listen(listen_fd, LISTENQ_LEN);

    cout << "Echo server (PID = " << getpid() << ", listen_fd = " << listen_fd <<
            ") is waiting connection on port " << port_listen_to << "..." << endl;

    // =============== Принятие соединения с клиентом ===============
    // Универсальная структура "sockaddr_storage" нужна для сохранения информации о подключившемся клиенте.
    sockaddr_storage client_address{};  // используем универсальную структуру адреса
    socklen_t client_address_len = sizeof(client_address);

    // Устанавливаем (регистрируем) обработчик SIGCHLD для обработки зомбированных процессов
    // перед циклом обработки входящих соединений от клиентов
    set_signal_handler();

    for (;;) {
        connected_fd = accept(listen_fd, (struct sockaddr*) &client_address, &client_address_len);
        if (connected_fd < 0) {
            if (errno == EINTR) {
                continue;  // назад в цикл for
            } else {
                cerr << "accept error: " << strerror(errno) << endl;
                exit(EXIT_FAILURE);
            }
        }
        // =============== Вывод информации о подключенном клиенте ===============
        print_info_about_connected_client(client_address);

        // =============== Обработка каждого подсоединившегося клиента в отдельном процессе ===============
        if (pid_t child_pid = Fork(); child_pid == 0) {
            handle_client_http_request(listen_fd, connected_fd, http_data);
        }

        // Родитель закрывает клиентский сокет, т.к. обработка соединения выполняется concurrently дочерним процессом.
         Close(connected_fd);
    }
}