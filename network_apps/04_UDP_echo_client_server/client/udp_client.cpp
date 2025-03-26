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
    cout << "Advanced udp client with connected socket." << endl;

    FILE* fp = stdin;

    // в клиенте должны быть заданы (через аргументы командной строки) IP-адрес сервера и номер порта для вызова "sendto"
    auto [server_ip, server_port] = echo_client_check_arguments(argc, argv);

    int sock_fd = Socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in server_address = get_filled_address_structure(server_ip, server_port);
    udp_client_echo(fp, sock_fd, (struct sockaddr*) &server_address, sizeof(server_address));

    return 0;
}
