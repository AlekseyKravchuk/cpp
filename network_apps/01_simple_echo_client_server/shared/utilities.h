#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

void echo_client_check_arguments(int argc,
                                 char* argv[],
                                 std::string& server_ip,
                                 uint16_t& server_port);

void server_check_arguments(int argc,
                            char* argv[],
                            uint16_t& port_listen_to,
                            std::string& file_path);

void echo_server_check_arguments(int argc,
                                 char* argv[],
                                 uint16_t& port_listen_to);

//std::string get_content(const std::string& file_path);

std::tuple<std::string, uint16_t> get_ip_port_from_addr_struct(sockaddr_storage& client_address);

//void handle_client(int connected_fd, const std::string& message);

ssize_t write_n_bytes_to_fd(int fd, const void* vptr, size_t n);

void server_str_echo(int sock_fd);

ssize_t my_read(int fd, char* ptr);

ssize_t readline(int fd, void* vptr, size_t max_len);

void client_str_echo(FILE* fp, int sock_fd);
