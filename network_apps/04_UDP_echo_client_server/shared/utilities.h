#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/stat.h>

void sigchld_handler(int signal_number);

void set_signal_handler();

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

void print_info_about_connected_client(sockaddr_storage& client_address);

void print_client_info(const std::string& server_ip, uint16_t server_port, int socket_fd);

//std::string get_content(const std::string& file_path);

std::tuple<std::string, uint16_t> get_ip_port_from_addr_struct(sockaddr_storage& client_address);

//void handle_client(int connected_fd, const std::string& message);

ssize_t write_n_bytes_to_sock_fd(int sock_fd, const void* buf_start, size_t n);

void server_str_echo(int sock_fd);

void udp_srv_echo(int sock_fd, struct sockaddr_storage* udp_client_addr, socklen_t* udp_client_addr_len);

ssize_t my_read(int sock_fd, char* ch_ptr);

ssize_t readline(int sock_fd, void* ptr_to_recv_buf, size_t max_len);

void client_str_echo(FILE* stdin_file, int sock_fd);

void client_str_echo_using_epoll(FILE* fp, int sock_fd);

int Select(int n_fds, fd_set* read_fds, fd_set* write_fds, fd_set* except_fds, struct timeval* timeout);

void set_nonblocking(int sock_fd);

bool file_is_regular(int file_fd);
