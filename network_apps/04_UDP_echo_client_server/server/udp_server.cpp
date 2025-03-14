/*
 * UDP echo-server
 */

#include <sys/socket.h>  // socket(...), recv(...)
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...), INET_ADDRSTRLEN

#include "wrappers.h"
#include "utilities.h"
#include "parsing.h"

using namespace std;

int main(int argc, char* argv[]) {
    int server_sock_fd;
    const string udp_serv_ip = "127.0.0.15";

    // TODO: сделать так, чтобы сервер UDP создал по одному сокету для каждого IP-адреса, сконфигурированного на хосте,
    //       связал с помощью bind() этот IP-адрес с сокетом, вызвал функцию epoll для каждого из этих сокетов, ожидая,
    //       когда какой-либо из них станет доступен для чтения, а затем ответит с этого готового для чтения сокета.
    //       Раздел 22.6 (Стивенс)
    // const string udp_serv_ip = "192.168.1.147";
    uint16_t udp_serv_port = 0;

    echo_server_check_arguments(argc, argv, udp_serv_port);
    server_sock_fd = Socket(AF_INET, SOCK_DGRAM, 0); // создаем UDP-сокет

    struct sockaddr_in server_address = {
            .sin_family = AF_INET,
            .sin_port = htons(udp_serv_port),
            .sin_addr = {},
            .sin_zero = {}
    };

    Inet_pton(AF_INET, udp_serv_ip.c_str(), &server_address.sin_addr);  // converts IP addresses from text to binary form
    Bind(server_sock_fd, (struct sockaddr*) &server_address, sizeof(server_address));

    sockaddr_storage client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);

    // Функция "udp_srv_echo" никогда не завершается.
    // Поскольку UDP - это протокол, не ориентированный на установление соединения, в нем не существует никаких
    // признаков конца файла (которые активно используются в TCP).
    udp_srv_echo(server_sock_fd, &client_addr, &client_addr_len);
}