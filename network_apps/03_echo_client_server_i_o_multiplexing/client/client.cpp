#include <vector>
#include <sys/socket.h>  // socket(), recv()
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)
#include <chrono>
#include <thread>
#include <memory>

#include "wrappers.h"
#include "utilities.h"
#include "parsing.h"

using namespace std;
using namespace chrono_literals;

int main(int argc, char* argv[]) {
    string server_ip{};
    uint16_t server_port{};
    constexpr size_t MAX_CONNECTIONS = 5;
    vector<int> client_sockets(MAX_CONNECTIONS);
    FILE* fp = stdin;  // Возможные значения fp: 0 (in case of stdin), N (in case of file redirection)

    echo_client_check_arguments(argc, argv, server_ip, server_port);

    // Частично заполняем server_address типа "sockaddr_in":
    sockaddr_in server_address{
            .sin_family = AF_INET,
            .sin_port = htons(server_port),
            .sin_addr = {},
            .sin_zero = {}
    };

    // Преобразование IP-адреса из точечно-десятичной нотации в двоичный вид (network-byte order)
    Inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr);

    // инициируем установление "MAX_CONNECTIONS" соединений
    for (size_t i = 0; i < MAX_CONNECTIONS; ++i) {
        client_sockets[i] = Socket(AF_INET, SOCK_STREAM, 0);
        Connect(client_sockets[i], (sockaddr*) &server_address, sizeof(server_address));
        print_client_info(server_ip, server_port, client_sockets[i]);

        // Реализация FILE* (к которому относится и stdin) предполагает наличие файлового указателя (offset),
        // который изменяется при чтении.
        // Перед запуском клиентов создаем копии файловых дескрипторов, чтобы для каждой итерации
        // был свой файловый дескриптор со своим указателем (the current file offset), указывающим на начало файла.
        int new_fd = (i == 0) ? fileno(fp) : dup(fileno(fp));
        lseek(new_fd, 0, SEEK_SET); // get back to the beginning of file
        FILE* new_fp = fdopen(new_fd, "r");

        client_str_echo_using_epoll(new_fp, client_sockets[i]);  // Выполняем все необходимые действия со стороны клиента.

        shutdown(client_sockets[i], SHUT_WR);        // Закрываем записывающую половину соединения

    }

    return 0;
}
