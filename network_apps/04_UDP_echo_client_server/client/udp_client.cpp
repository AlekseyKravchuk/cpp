/*
 * UDP echo-client
 */

#include <sys/socket.h>  // socket(), recv()
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)

#include "wrappers.h"
#include "utilities.h"
#include "parsing.h"

using namespace std;

int main(int argc, char* argv[]) {
    string server_ip{};
    uint16_t server_port{};
    FILE* fp = stdin;

    echo_client_check_arguments(argc, argv, server_ip, server_port);

    // Частично заполняем server_address типа "sockaddr_in":
    sockaddr_in server_address{
            .sin_family = AF_INET,
            .sin_port = htons(server_port),
            .sin_addr = {},
            .sin_zero = {}
    };

    // Преобразование IP-адреса из точечно-десятичной нотации в двоичный вид (network-byte order)
    Inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr);

    int sock_fd = Socket(AF_INET, SOCK_DGRAM, 0);

    udp_client_echo(fp, sock_fd, (struct sockaddr*) &server_address, sizeof(server_address));

    return 0;
}
