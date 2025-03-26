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
//    const string udp_serv_ip = "192.168.1.147";

    // TODO: сделать так, чтобы сервер UDP создал по одному сокету для каждого IP-адреса, сконфигурированного на хосте,
    //       связал с помощью bind() этот IP-адрес с сокетом, вызвал функцию epoll для каждого из этих сокетов, ожидая,
    //       когда какой-либо из них станет доступен для чтения, а затем ответит с этого готового для чтения сокета.
    //       Раздел 22.6 (Стивенс)

    uint16_t udp_serv_port = echo_server_check_arguments(argc, argv);
    server_sock_fd = Socket(AF_INET, SOCK_DGRAM, 0); // создаем UDP-сокет
    sockaddr_in server_address = get_filled_address_structure(udp_serv_ip, udp_serv_port);
    Bind(server_sock_fd, (struct sockaddr*) &server_address, sizeof(server_address));

    // используем универсальную структуру адреса - теперь к нам могут подключаться как IPv4-, так и IPv6-клиенты
    sockaddr_storage client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);

    // Функция "udp_srv_echo" никогда не завершается.
    // Поскольку UDP - это протокол, не ориентированный на установление соединения, в нем не существует никаких
    // признаков конца файла (которые активно используются в TCP).
    udp_srv_echo(server_sock_fd, &client_addr, &client_addr_len);
}