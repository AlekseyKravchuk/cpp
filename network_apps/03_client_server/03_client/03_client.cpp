/*
 * Simple TCP-client that prints out any data that it gets from a remote connection
 */

#include <iostream>

#include <sys/types.h>   // socket(...)
#include <sys/socket.h>  // socket(...), recv(...)

#include <cstdlib>       // exit(...)
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)
#include <arpa/inet.h>   // inet_pton(...)
#include <cstdio>
#include <unistd.h>      // close(...)


using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "usage: a.out <IP_address> <port>" << endl;
        perror("wrong number of arguments");
        exit(EXIT_FAILURE);
    }

    const size_t MAX_BUFFER_LEN = 4096;  // max buffer size to hold a response from the server

    // ============== create a socket ==============
    int socket_fd;
    if ( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket(...) error");
        exit(EXIT_FAILURE);
    }

    // specify an address for the socket; htons: host to network short
    struct sockaddr_in srv_address{
        .sin_family = AF_INET,
        .sin_port = htons(static_cast<uint16_t>(stoi(argv[2]))),
        .sin_zero = {}
    };

    if (inet_pton(AF_INET, argv[1], &srv_address.sin_addr) <= 0) {
        cerr << "inet_pton error for " << argv[1] << " IP address" << endl;
        perror("inet_pton(...) error");
        exit(EXIT_FAILURE);
    }

    // ================= connect ==================
    int connection_status = connect(socket_fd,
                                    (struct sockaddr*) &srv_address,
                                    sizeof(srv_address));
    if (connection_status < 0) {
        cerr << "There was an error making a connection to the remote socket "
             << argv[1] << ':' << argv[2] << endl;
        perror("connect(...) error");
        exit(EXIT_FAILURE);
    }

    // ============== receive data ================
    char srv_response[MAX_BUFFER_LEN];
//    ssize_t count = recv(socket_fd, &srv_response, MAX_BUFFER_LEN, 0); // receive data from the server
    recv(socket_fd, &srv_response, MAX_BUFFER_LEN, 0); // receive data from the server

    // print out the server's response
    printf("The server sent the data: %s\n", srv_response);

    // ============= close connection =============
    // After we done, we close the socket with a close(...) function
    close(socket_fd);

    return 0;
}
