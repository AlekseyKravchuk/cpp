#pragma once

#include <cstdio>        // perror
#include <cstdlib>       // exit, EXIT_FAILURE
#include <cstring>       // memset
#include <sys/socket.h>  // socket
#include <arpa/inet.h>
#include <unistd.h>      // write

int Socket(int family, int type, int protocol);

void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

int Inet_pton(int af, const char *src, void *dst);

void Listen(int sockfd, int backlog);

int Accept(int fd, struct sockaddr* sa, socklen_t* salenptr);

void Write(int fd, void* ptr, size_t nbytes);

void Close(int fd);

