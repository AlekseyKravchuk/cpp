#pragma once

#include <algorithm>
#include <iostream>
#include <tuple>
#include <boost/program_options.hpp>

std::tuple<std::string, uint16_t> echo_client_check_arguments(int argc, char* argv[]);

uint16_t echo_server_check_arguments(int argc, char* argv[]);