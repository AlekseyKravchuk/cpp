#include <iostream>
#include <cstring>       // memset
#include <sys/socket.h>  // socket
#include <arpa/inet.h>
#include <unistd.h>      // close


#include "utilities.h"

using namespace std;

int Socket(int domain, int type, int protocol) {
    int socket_fd;

    if ((socket_fd = socket(domain, type, protocol)) < 0) {
        cerr << "socket function error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

int Inet_pton(int af, const char* src, void* dst) {
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

const char* Inet_ntop(int af, const void* src,
                      char* dst, socklen_t size) {
    const char* ptr = inet_ntop(af, src, dst, size);
    if (ptr == nullptr) {
        cerr << "Client address could not be converted to string representation (inet_ntop): "
             << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return dst;
}

int Connect(int socket_fd,
            const struct sockaddr* server_address_casted,
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

ssize_t Recv(int socket_fd, void* buffer, size_t buffer_size, int flags) {

    // Если recv(read) возвращает 0, это значит, что сервер закрыл соединение.
    ssize_t bytes_read = recv(socket_fd, buffer, buffer_size, 0);

    if (bytes_read == 0) {
        cout << "Connection closed by server." << endl;
    } else if (bytes_read < 0) {
        cerr << "socket read error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return bytes_read;
}

void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    int result = bind(sockfd, addr, addrlen);

    if (result == -1) {
        cerr << "bind error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Listen(int sock_fd, int backlog) {
    char* ptr = getenv("LISTENQ");
    char** end_ptr = nullptr;

    // May be override 2nd argument with environment variable
    if (ptr != nullptr) {
        backlog = static_cast<int>(strtol(ptr, end_ptr, 10));
    }

    int result = listen(sock_fd, backlog);

    if (result == -1) {
        cerr << "listen failed: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

int Accept(int listen_fd, struct sockaddr* sa, socklen_t* salenptr) {
    int client_socket_fd = accept(listen_fd, sa, salenptr);

    if (client_socket_fd < 0) {
        cerr << "accept connection error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return client_socket_fd;
}

ssize_t Send(int socket_fd, const void* buf, size_t len, int flags = 0) {
    ssize_t num_bytes_sent = 0;

    // !!! Успешное завершение "send()" ещё не означает, что процесс на другом конце соединения получил отправленные данные!!!
    // Всё, что гарантирует "send()" - это отсутствие ошибок при передаче данных сетевым драйверам.
    if ((num_bytes_sent = send(socket_fd, buf, len, flags)) < 0) {
        cerr << "Sending data error (send call): " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return num_bytes_sent;
}

void Close(int fd) {
    if (close(fd) == -1) {
        cerr << "close error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Write_n_bytes_to_sock_fd(int fd, void* ptr, size_t n_bytes) {
    if (write_n_bytes_to_fd(fd, ptr, n_bytes) != n_bytes) {
        cerr << "write_n_bytes_to_fd error" << endl;
        exit(EXIT_FAILURE);
    }
}

char* Fgets(char* ptr, int n, FILE* stream) {
    char* rptr;

    if ((rptr = fgets(ptr, n, stream)) == nullptr && ferror(stream)) {
        cerr << "fgets error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return (rptr);
}

void Fputs(const char* ptr, FILE* stream) {
    if (fputs(ptr, stream) == EOF) {
        cerr << "fputs error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

ssize_t Readline(int fd, void* ptr, size_t max_len) {
    ssize_t n;

    if ((n = readline(fd, ptr, max_len)) < 0) {
        cerr << "readline error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return (n);
}

