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
    FILE* fp = stdin;  // Можно передать stdin или файл, например fopen("file.txt", "r");

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
    // TODO: решить проблему с переиспользованием открытого файла для чтения несколькими потоками.
    // terminate called without an active exception
    // Aborted (core dumped)
    for (size_t i = 0; i < MAX_CONNECTIONS; ++i) {
        client_sockets[i] = Socket(AF_INET, SOCK_STREAM, 0);
        Connect(client_sockets[i], (sockaddr*) &server_address, sizeof(server_address));
        print_client_info(server_ip, server_port, client_sockets[i]);
        FILE* new_fp = fdopen(dup(fileno(fp)), "r");
        client_str_echo(new_fp, client_sockets[i]);  // Выполняем все необходимые действия со стороны клиента.
        shutdown(client_sockets[i], SHUT_WR);        // Закрываем записывающую половину соединения

    }

    return 0;
}
