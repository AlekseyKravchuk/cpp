/*
 * Simple ECHO TCP-client
 */

#include <iostream>
#include <vector>
#include <sys/socket.h>  // socket(), recv()
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)

#include "wrappers.h"
#include "utilities.h"
#include "parsing.h"

using namespace std;

int main(int argc, char* argv[]) {
    string server_ip{};
    uint16_t server_port{};
    constexpr size_t MAX_CONNECTIONS = 5;
    vector<int> client_sockets{MAX_CONNECTIONS};

    echo_client_check_arguments(argc, argv, server_ip, server_port);

    /*
     * Клиент устанавливает "MAX_CONNECTIONS" соединений, а задем использует первое из них "client_sockets[0]"
     * для вызова функции "client_str_echo". Несколько соединений устанавливаются для того, чтобы породить от
     * concurrent-сервера множество дочерних процессов
    */
    for (size_t i = 0; i < MAX_CONNECTIONS; ++i) {
        client_sockets[i] = Socket(AF_INET, SOCK_STREAM, 0);

        // Частично заполняем server_address типа "sockaddr_in":
        sockaddr_in server_address{
                .sin_family = AF_INET,
                .sin_port = htons(server_port),
                .sin_addr = {},
                .sin_zero = {}
        };

        // Преобразование IP-адреса из точечно-десятичной нотации в двоичный вид (network-byte order)
        Inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr);

        Connect(client_sockets[i], (sockaddr*) &server_address, sizeof(server_address));
        print_client_info(server_ip, server_port, client_sockets[i]);
    }


    // Эта функция выполняет все необходимые действия со стороны клиента.
    /*client_str_echo(stdin, socket_fd);*/
    client_str_echo(stdin, client_sockets[0]);

    // Явно вызываем close() для завершения соединения на стороне клиента.
    // Это нужно для того, чтобы  освободить все ресурсы, связанные с этим соединением, включая файловые дескрипторы.
    /*Close(socket_fd);*/
    Close(client_sockets[0]);

    // В Linux при завершении процесса ядро автоматически закрывает все открытые файловые дескрипторы, включая сокеты.
    // Если процесс клиента завершился, ядро закроет сокет, и в результате будет отправлен [FIN, ACK].
    return 0;
}
