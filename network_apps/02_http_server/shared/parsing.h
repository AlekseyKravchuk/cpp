#pragma once

#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>

void echo_client_check_arguments(int argc,
                                 char* argv[],
                                 std::string& server_ip,
                                 uint16_t& server_port);

void http_server_check_arguments(int argc,
                                 char* argv[],
                                 uint16_t& port_listen_to);