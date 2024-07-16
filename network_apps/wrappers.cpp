#include <cstdio>        // perror
#include <cstdlib>       // exit, EXIT_FAILURE
#include <cstring>       // memset
#include <sys/socket.h>  // socket
#include <arpa/inet.h>
#include <unistd.h>      // write

int Socket(int family, int type, int protocol) {
    int socket_fd;

    if ((socket_fd = socket(family, type, protocol)) < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    int res = bind(sockfd, addr, addrlen);
    if (res == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
}

int Inet_pton(int af, const char *src, void *dst) {
    int res = inet_pton(af, src, dst);

    if (res <= 0) {
        perror("inet_pton error");
        exit(EXIT_FAILURE);
    }

    return res;
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

void Write(int fd, void* ptr, size_t nbytes) {
    if (write(fd, ptr, nbytes) != nbytes) {
        perror("write error");
        exit(EXIT_FAILURE);
    }
}

void Close(int fd) {
    if (close(fd) == -1) {
        perror("close error");
        exit(EXIT_FAILURE);
    }
}

