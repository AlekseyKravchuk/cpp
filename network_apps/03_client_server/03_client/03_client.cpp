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

int main(int argc, char* argv[]) {
//    if (argc != 3) {
//        cerr << "usage: a.out <server_IP_address> <server_port>" << endl;
//        perror("wrong number of arguments");
//        exit(EXIT_FAILURE);
//    }

    // ============== Connection parameters ==============
    const size_t MAX_BUFFER_LEN = 4096;  // max buffer size to hold a response from the server
    char buffer[MAX_BUFFER_LEN];
    const string srv_ip = "127.0.0.1";
    const int srv_port = 43180;

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
        srv_response.append(buffer, bytes_received);
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
