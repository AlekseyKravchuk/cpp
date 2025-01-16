//
// Created by kav on 16.01.25.
//
#include <iostream>

#include <cstdio>        // perror
#include <cstdlib>       // exit, EXIT_FAILURE
#include <cstring>       // memset
#include <sys/socket.h>  // socket
#include <arpa/inet.h>
#include <unistd.h>      // close

using namespace std;

int Socket(int domain, int type, int protocol) {
    int socket_fd;

    if ((socket_fd = socket(domain, type, protocol)) < 0) {
        cerr << "socket function error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

int Inet_pton(int af, const char *src, void *dst) {
    int result = inet_pton(af, src, dst);

    if (result == 0) {
        cerr << "Invalid server IP-address: " << src << endl;
        exit(EXIT_FAILURE);
    } else if (result == -1) {
        cerr << "Address family is not supported: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return result;
}

int Connect(int socket_fd,
            const struct sockaddr *server_address_casted,
            socklen_t addrlen) {

    // If the connection or binding succeeds, zero is returned.  On error, -1 is returned, and errno is set appropriately.
    int result = connect(socket_fd, server_address_casted, addrlen);

    if (result < 0) {
        cerr << "There was an error making a connection to the remote socket (\"connect\" function): "
             << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return result;
}

ssize_t Recv(int socket_fd, void *buffer, size_t buffer_size, int flags) {

    // Если recv(read) возвращает 0, это значит, что сервер закрыл соединение.
    ssize_t bytes_read = recv(socket_fd, buffer, buffer_size, 0);

    if (bytes_read == 0) {
        cout << "Connection closed by server." << endl;
    } else if (bytes_read < 0) {
        cerr << "Socket read error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return bytes_read;
}

void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    int res = bind(sockfd, addr, addrlen);
    if (res == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
}


void Listen(int sockfd, int backlog) {
    int res = listen(sockfd, backlog);

    if (res == -1) {
        perror("listen() failed.");
        exit(EXIT_FAILURE);
    }
}

int Accept(int fd, struct sockaddr* sa, socklen_t* salenptr) {
    int res;

    if ((res = accept(fd, sa, salenptr)) < 0) {
        perror("accept failed.");
        exit(EXIT_FAILURE);
    }

    return res;
}

ssize_t Send(int socket_fd, const void *buf, size_t len, int flags=0) {
    ssize_t num_bytes_sent = 0;

    if ( (num_bytes_sent = send(socket_fd, buf, len, flags)) < 0) {
        cerr << "Sending data error (send call): " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return num_bytes_sent;
}

void Close(int fd) {
    if (close(fd) == -1) {
        perror("close error");
        exit(EXIT_FAILURE);
    }
}

