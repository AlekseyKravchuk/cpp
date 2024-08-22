/*
 * Simple TCP-client that prints out any data that it gets from a remote connection
 */

#include <iostream>

#include <sys/socket.h>  // socket(...), recv(...)
#include <cstdlib>       // exit(...)
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)
#include <arpa/inet.h>   // inet_pton(...)
#include <cstdio>
#include <unistd.h>      // close(...)
#include <netdb.h>       // struct hostent *gethostbyname(const char *name);
#include <cstring>       // std::strerror - analogue of std::perror()

using namespace std;

/*
 * Приложение, собирающееся использовать ТРАНСПОРТНЫЙ уровень для обмена данными, должно предоставить интерфейсу
 * сокетов (socket)  следующую информацию:
 * 1) указание на протокол, который надо использовать при обмене данными:
 *        socket (AF_*, SOCK_STREAM, 0);  // 0 means 'auto choice' protocol number
 *                  эквивалентно
 *        socket (AF_*, SOCK_STREAM, 6);  // 6 means 'TCP' protocol inside IP packet
 *
 *    кроме потоковой передачи данных бывают ещё и другие:
 *        SOCK_STREAM - bidirectional data streaming
 *        SOCK_DGRAM  - unidirectional short messages (UDP)
 *        SOCK_RAW    - IP level custom packets
 *        SOCK_PACKET - Linux-only Ethernet-level custom frames
 *  2) сетевой адрес другого абонента (IP-адрес)
 *  3) номер порта, который будет использовать транспортный уровень
 *  4) сами данные в виде массива байтов
 */

int main(int argc, char* argv[]) {
    string srv_ip = "127.0.0.1";
    uint16_t srv_port = 43180;

    if (argc == 1) {
        cout << "parameterless usage:"
             << argv[0] << "  <server_IP_address> = " << srv_ip << " <server_port> = " << srv_port << endl;
    } else if (argc == 3) {
        srv_ip = argv[1];
        srv_port = static_cast<uint16_t>(std::stoul(argv[2]));
    } else {
        cerr << "usage: " << argv[0] << " <server_IP_address> <server_port>" << endl
             << "OR (parameterless):\n"
             << "usage: " << argv[0] << endl;
        perror("wrong number of arguments");
        exit(EXIT_FAILURE);
    }

    // ============== Connection parameters ==============
    const size_t MAX_BUFFER_LEN = 4096;  // max buffer size to hold a response from the server
    char buffer[MAX_BUFFER_LEN];

    // ============== Create a socket ==============
    int client_socket_fd;

    // функция "socket" создает потоковый TCP-сокет (AF_INET - это красивое название для обычного сокета)
    // SOCK_STREAM <=> TCP; SOCK_DGRAM <=> UDP
    if ((client_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Creating socket error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    struct hostent* hostinfo = nullptr;
    hostinfo = gethostbyname(srv_ip.c_str());
    if (hostinfo == nullptr) {
        perror("gethostbyname(...) error");
        exit(EXIT_FAILURE);
    }

    // specify an address for the socket (use designated initializers);
    // htons: host to network short
    // host might be connected to multiple networks and have different addresses on each one
    struct sockaddr_in srv_addr{
        .sin_family = AF_INET,
        .sin_port = htons(srv_port),
        .sin_addr = *(struct in_addr*) hostinfo->h_addr_list[0],
        .sin_zero = {}
    };

    if (inet_pton(AF_INET, srv_ip.c_str(), &srv_addr.sin_addr) <= 0) {
        cerr << "Wrong server IP-address: " << strerror(errno) << endl;
        close(client_socket_fd);
        exit(EXIT_FAILURE);
    }

    // ================= connect ==================
    int connection_status = connect(client_socket_fd,
                                    (struct sockaddr*) &srv_addr,
                                    sizeof(srv_addr));
    // TODO: fix error "There was an error making a connection to the remote socket 127.0.0.1:43180"
    if (connection_status < 0) {
        std::cerr << "Connect error: " << std::strerror(errno) << std::endl;
        cerr << "There was an error making a connection to the remote socket "
             << srv_ip << ':' << srv_port << endl;
        close(client_socket_fd);
        exit(EXIT_FAILURE);
    }

    // ============== receive data ================
    string srv_response;
    ssize_t bytes_received;
    while ((bytes_received = recv(client_socket_fd, buffer, sizeof(buffer), 0) ) > 0) {
        srv_response.append(buffer, static_cast<size_t>(bytes_received));
    }

    if (bytes_received < 0) {
        cerr << "Reading data error: " << strerror(errno) << endl;
    } else {
        cout << "Data received from the server " << srv_ip << ":" << srv_port
             << ": " << srv_response << endl;
    }

    // print out the server's response
    printf("The server sent the data: %s\n", buffer);

    // ============= close connection =============
    close(client_socket_fd);

    return 0;
}
