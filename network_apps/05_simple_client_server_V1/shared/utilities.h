#pragma once

#include <string>

#include <sys/socket.h>
#include <netinet/in.h>

std::string info_about_connected_client(sockaddr_storage& client_address);
