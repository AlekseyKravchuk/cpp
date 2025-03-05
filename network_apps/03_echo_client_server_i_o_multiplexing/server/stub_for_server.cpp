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

#define MAX_BUF_SIZE 4096

int main(int argc, char** argv) {
    uint16_t port_listen_to = 0;
    constexpr int LISTENQ_LEN = 100;
    int i, max_i, max_fd, listen_fd, connected_fd, current_sock_fd;
    int n_ready;
    int clients[FD_SETSIZE];
    ssize_t n_bytes;
    fd_set read_set, all_set;
    char buf[MAX_BUF_SIZE];

    // Универсальная структура "sockaddr_storage" нужна для сохранения информации о подключившемся клиенте.
    sockaddr_storage client_addr{};  // используем универсальную структуру адреса
    socklen_t client_addr_len = sizeof(client_addr);

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

    max_fd = listen_fd;            /* initialize */
    max_i = -1;                    /* index into clients[] array */

    for (i = 0; i < FD_SETSIZE; i++) {
        clients[i] = -1;            /* -1 indicates available entry */
    }

    FD_ZERO(&all_set);
    FD_SET(listen_fd, &all_set);

    for (;;) {
        read_set = all_set;        /* structure assignment */

        // select() ждет установления соединения с новым клиентом,
        // или пока на существующее соединение не придет сегмент FIN или RST
        n_ready = Select(max_fd + 1, &read_set, nullptr, nullptr, nullptr);

        // если прослушиваемый сокет готов для чтения => новое соединение установлено
        if (FD_ISSET(listen_fd, &read_set)) {
            client_addr_len = sizeof(client_addr);
            connected_fd = Accept(listen_fd, (struct sockaddr*) &client_addr, &client_addr_len);
            print_info_about_connected_client(client_addr);  // Вывод информации о подключении клиента

            for (i = 0; i < FD_SETSIZE; i++) {  // FD_SETSIZE: maximum number of file descriptors in `fd_set'
                if (clients[i] < 0) {
                    clients[i] = connected_fd;  // сохраняем дескриптор подключившегося клиента
                    break;
                }
            }

            if (i == FD_SETSIZE) {
                cerr << "too many clients" << endl;
                exit(EXIT_FAILURE);
            }

            FD_SET(connected_fd, &all_set);    /* add new descriptor to set */

            if (connected_fd > max_fd) {
                max_fd = connected_fd;            /* for select */
            }

            if (i > max_i) {
                max_i = i;                /* max index in clients[] array */
            }


            if (--n_ready <= 0) {
                continue;                /* no more readable descriptors */
            }
        }

        // Проверяем каждое существующее клиентское соединение на предмет того, содержится ли его дескриптор
        // в наборе дескрипторов, возвращаемом функцией select()
        for (i = 0; i <= max_i; i++) {    /* check all clients for data */
            current_sock_fd = clients[i];
            if (current_sock_fd < 0) {
                continue;
            }

            // если да, то из клиентского соединения считывается строка и отправляется обратно клиенту
            if (FD_ISSET(current_sock_fd, &read_set)) {
                n_bytes = Read(current_sock_fd, buf, MAX_BUF_SIZE);

                if (n_bytes == 0) {
                    Close(current_sock_fd);  // connection closed by clients
                    FD_CLR(current_sock_fd, &all_set);
                    clients[i] = -1;
                } else {
                    Write_n_bytes_to_sock_fd(current_sock_fd, buf, n_bytes);
                }

                if (--n_ready <= 0)
                    break;                /* no more readable descriptors */
            }
        }
    }
}

