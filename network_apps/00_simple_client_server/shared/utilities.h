#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

std::tuple<std::string, uint16_t> get_ip_port_from_addr_struct(sockaddr_storage& client_address);

void handle_client(int connected_fd, const std::string& message);

void check_link_count(int fd);

//void handle_client_connection_concurrently(int listen_fd, int connected_fd, const std::string& message);