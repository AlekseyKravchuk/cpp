/*
 * !!! Simpliest protocol-dependent version of TCP client-server !!!
 */

#include <iostream>

#include <sys/socket.h>  // socket(), recv()
#include <cstdlib>       // exit()
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)
#include <cstdio>        // void perror( const char *s ); Prints a textual description of the error code currently stored in the system variable errno to stderr.
#include <cstring>       // void* memset( void* dest, int ch, std::size_t count );
#include <unistd.h>      // close(...)

#include "wrappers.h"

using namespace std;

int main(int argc, char* argv[]) {
    int socket_fd = 0;
    constexpr size_t buffer_size = 1024;
    char buffer[buffer_size];
    string srv_ip{};
    uint16_t srv_port{};

    if (argc != 3) {
        cerr << "usage: " << argv[0] << " <server_IP_address> <server_port>" << endl;
        perror("wrong number of arguments");
        exit(EXIT_FAILURE);
    } else {
        srv_ip = argv[1];
        srv_port = static_cast<uint16_t>(std::stoul(argv[2]));
    }

    // ============== Create a socket ==============
    socket_fd = Socket(AF_INET, SOCK_STREAM, 0);

    // Частично заполняем server_address типа "sockaddr_in":
    sockaddr_in server_address {
            .sin_family = AF_INET,
            .sin_port = htons(srv_port),
            .sin_addr = {},
            .sin_zero = {}
    };

    // Преобразование IP-адреса из точечно-десятичной нотации в двоичный вид ()
    Inet_pton(AF_INET, srv_ip.c_str(), &server_address.sin_addr);

    Connect(socket_fd, (sockaddr*) &server_address, sizeof(server_address));

    // получаем данные от сервера (recv от "receive")
    std::string complete_message;
    for (;;) {
        ssize_t bytes_read = Recv(socket_fd, buffer, buffer_size, 0);
        if (bytes_read == 0) {  // Если recv вернула 0, значит сервер закрыл соединение.
            break;
        }

        complete_message.append(buffer, static_cast<size_t>(bytes_read));
    }

    // Выводим полученное сообщение.
    cout << "The message sent by the server: " <<  complete_message << endl;

    // ============= close connection =============
    if (socket_fd > 0) {
        close(socket_fd);
    } else {
        cerr << "Client socket <= 0." << endl;
    }

    return 0;
}
