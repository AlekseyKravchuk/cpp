#pragma once

#include <cstdio>        // perror
#include <cstdlib>       // exit, EXIT_FAILURE
#include <cstring>       // memset
#include <sys/socket.h>  // socket
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>      // write
#include <fstream>

int Socket(int domain, int type, int protocol);

// wrapper fo inet_ptor: convert IPv4 and IPv6 addresses from text to binary form (to network byte order)
int Inet_pton(int af, const char *src, void *dst);

int Connect(int socket_fd,
            const struct sockaddr *addr,
            socklen_t addrlen);

ssize_t Recv(int socket_fd, void *buffer, size_t buffer_size, int flags);

void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Accept(int fd, struct sockaddr* sa, socklen_t* salenptr);

ssize_t Send(int socket_fd, const void *buf, size_t len, int flags=0);

void Close(int fd);

void client_check_arguments(int argc, char* argv[], std::string& server_ip, uint16_t& server_port);

//void server_check_arguments(int argc, char* argv[], uint16_t& port_listen_to);
int server_check_arguments(int argc, char* argv[], uint16_t& port_listen_to, std::string& filepath);

std::string get_content(const std::string& file_path);


