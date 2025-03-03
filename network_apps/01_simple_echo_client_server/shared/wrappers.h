#pragma once

#include <cstdio>        // perror
#include <cstdlib>       // exit, EXIT_FAILURE
#include <cstring>       // memset
#include <sys/socket.h>  // socket
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>      // write(), close()
#include <fstream>

int Socket(int domain, int type, int protocol);

// wrapper fo inet_pton: convert IPv4 and IPv6 addresses from text to binary form (to network byte order)
int Inet_pton(int af, const char *src, void *dst);

const char* Inet_ntop(int af, const void *src,
                      char *dst, socklen_t size);

int Connect(int socket_fd,
            const struct sockaddr *addr,
            socklen_t addrlen);

ssize_t Recv(int socket_fd, void *buffer, size_t buffer_size, int flags);

void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

void Listen(int sock_fd, int backlog);

int Accept(int fd, struct sockaddr* sa, socklen_t* salenptr);

ssize_t Send(int socket_fd, const void *buf, size_t len, int flags=0);

void Close(int fd);

//void restrict_to_iface(int server_fd,
//                       int level,
//                       int optname,
//                       const void* optval,
//                       socklen_t optlen);

void Write_n_bytes_to_sock_fd(int sock_fd, const void* buf_start, size_t n_bytes);

char* Fgets(char* str_buf, int n, FILE* file_stream);

FILE* Fopen(const char* filename, const char* mode);

void Fputs(const char* ptr, FILE* stream);

ssize_t Write(int fd, const void* recv_buf, size_t recv_buf_len);

ssize_t Readline(int fd, void* ptr_to_buf, size_t max_len);

pid_t Fork();

ssize_t Read(int fd, void* ptr, size_t n_bytes);

void Shutdown(int fd, int how);
