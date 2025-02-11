/*
 * Simple ECHO TCP-client
 */

#include <iostream>
#include <sys/socket.h>  // socket(), recv()
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)
#include <tuple>

#include "wrappers.h"
#include "utilities.h"

using namespace std;

int main(int argc, char* argv[]) {
    string server_ip{};
    uint16_t server_port{};

    echo_client_check_arguments(argc, argv, server_ip, server_port);

    // ============== Create a socket ==============
    int socket_fd = Socket(AF_INET, SOCK_STREAM, 0);

    // Частично заполняем server_address типа "sockaddr_in":
    sockaddr_in server_address{
            .sin_family = AF_INET,
            .sin_port = htons(server_port),
            .sin_addr = {},
            .sin_zero = {}
    };

    // Преобразование IP-адреса из точечно-десятичной нотации в двоичный вид (network-byte order)
    Inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr);

    Connect(socket_fd, (sockaddr*) &server_address, sizeof(server_address));

    // После успешного выполнения функции connect и возвращения управления в клиентский TCP-процесс, которые НЕ вызывал
    // bind(), функция getsockname возвращает IP адрес и номер локального порта, присвоенные ядром ОС.
    sockaddr_storage client_struct_address{};  // используем универсальную структуру адреса для адреса подключенного клиента
    socklen_t addr_len = sizeof(client_struct_address);
    getsockname(socket_fd, (struct sockaddr*) &client_struct_address, &addr_len);
    auto [client_ip, client_port] = get_ip_port_from_addr_struct(client_struct_address);

    std::cout << "Client " << client_ip << ":" << client_port
              << " (PID = " << getpid() << ") "
              << "is connected to server " << server_ip << ":" << server_port << endl;

    // Эта функция выполняет все необходимые действия со стороны клиента.
    client_str_echo(stdin, socket_fd);

    // Явно вызываем close() для завершения соединения на стороне клиента.
    // Это нужно для того, чтобы  освободить все ресурсы, связанные с этим соединением, включая файловые дескрипторы.
    Close(socket_fd);

    // В Linux при завершении процесса ядро автоматически закрывает все открытые файловые дескрипторы, включая сокеты.
    // Если процесс клиента завершился, ядро закроет сокет, и в результате будет отправлен [FIN, ACK].
    return 0;
}
