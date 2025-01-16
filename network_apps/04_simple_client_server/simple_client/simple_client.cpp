/*
 * !!! Simpliest protocol-dependent version of TCP client-server !!!
 */

#include <iostream>

#include <sys/socket.h>  // socket(), recv()
#include <cstdlib>       // exit()
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)
#include <arpa/inet.h>   // inet_pton(...)
#include <cstdio>        // void perror( const char *s ); Prints a textual description of the error code currently stored in the system variable errno to stderr.
#include <cstring>       // void* memset( void* dest, int ch, std::size_t count );
#include <unistd.h>      // close(...)

using namespace std;

int main(int argc, char* argv[]) {
    int sock_fd = 0;
    constexpr size_t buffer_size = 1024;
    char buffer[buffer_size];
    string srv_ip{};     // "127.0.0.5";
    uint16_t srv_port{}; // 43180;

    if (argc != 3) {
        cerr << "usage: " << argv[0] << " <server_IP_address> <server_port>" << endl;
        perror("wrong number of arguments");
        exit(EXIT_FAILURE);
    } else {
        srv_ip = argv[1];
        srv_port = static_cast<uint16_t>(std::stoul(argv[2]));
    }

    // ============== Create a socket ==============
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "socket function error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    // Частично заполняем server_address типа "sockaddr_in":
    sockaddr_in server_address {
            .sin_family = AF_INET,
            .sin_port = htons(srv_port),
            .sin_addr = {},
            .sin_zero = {}
    };

    // Преобразование IP-адреса из точечно-десятичной нотации в двоичный вид
    if (int res = inet_pton(AF_INET, srv_ip.c_str(), &server_address.sin_addr);
            res == 0) {
        cerr << "Invalid server IP-address: " << srv_ip << endl;
        exit(EXIT_FAILURE);
    } else if (res == -1) {
        cerr << "Address family is not supported: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (sockaddr*) &server_address, sizeof(server_address)) < 0) {
        cerr << "There was an error making a connection to the remote socket (\"connect\" function): "
             << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    // получаем данные от сервера (recv от "receive")
    std::string complete_message;
    for (;;) {
        // Если recv (read) возвращает 0, это значит, что сервер закрыл соединение.
        ssize_t bytes_read = recv(sock_fd, buffer, buffer_size, 0);

        if (bytes_read == 0) {
            cout << "Connection closed by server." << endl;
            break;  // Соединение закрыто, завершаем получение данных
        } else if (bytes_read < 0) {
            cerr << "Socket read error: " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }

        complete_message.append(buffer, static_cast<size_t>(bytes_read));
    }

    // Выводим полученное сообщение.
    cout << "The message sent by the server: " <<  complete_message << endl;

    // ============= close connection =============
    if (sock_fd > 0) {
        close(sock_fd);
    } else {
        cerr << "Client socket <= 0." << endl;
    }

    return 0;
}
