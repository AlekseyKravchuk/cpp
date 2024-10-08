#include <ctime>
#include <string>

#include "wrappers.h"

using namespace std;

int main() {
    const int MAX_BUFFER_SIZE = 4096; // max text line length
    string ip_address = "127.0.0.5";
    uint16_t port = 3400;
    int queue_length = 5;
    time_t ticks;
    char buffer[MAX_BUFFER_SIZE]{};

    int listen_fd = Socket(AF_INET, SOCK_STREAM, 0);
    int connection_fd = 0;

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    Inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr);

    Bind(listen_fd, (struct sockaddr* )&server_addr, sizeof(server_addr));

    Listen(listen_fd, queue_length);

    for ( ; ; ) {
        connection_fd = Accept(listen_fd, (struct sockaddr* ) nullptr, nullptr);

        ticks = time(nullptr);
        snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));
        Write(connection_fd, buffer, strlen(buffer));

        Close(connection_fd);
    }

    return 0;
}