/*
 * Simple TCP-server that listen on user-defined socket
 */

#include <iostream>

#include <sys/types.h>   // socket(...)
#include <sys/socket.h>  // socket(...), recv(...)

#include <cstdlib>       // exit(...)
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)
#include <arpa/inet.h>   // inet_pton(...)
#include <cstdio>
#include <unistd.h>      // close(...)

#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "usage: " << argv[0] << " <port_to_listen_to>" << endl;
        perror("wrong number of arguments");
        exit(EXIT_FAILURE);
    }

    // So first we will create a string to hold the data that we're going to send all the clients
//    const char* server_message = "You have reached the server";
    string server_message = "You have reached the server";

    // create the server socker
    int srv_socket = socket(AF_INET, SOCK_STREAM, 0);

    uint16_t port = htons(static_cast<uint16_t>(stoi(argv[1])));
    // define the server address
    struct sockaddr_in srv_address = {
            .sin_family = AF_INET,
            .sin_port = port,
            .sin_addr = {INADDR_ANY},
            .sin_zero = {}
    };

    // bind the socket to out specified IP and port
    bind(srv_socket, (struct sockaddr*) &srv_address, sizeof(srv_address));

    listen(srv_socket, 5);

    int client_socket;
    client_socket = accept(srv_socket, nullptr, nullptr);

    // send the message
    send(client_socket, server_message.c_str(), sizeof(server_message.c_str()), 0);

    // close the socket
    close(srv_socket);

    return 0;
}
