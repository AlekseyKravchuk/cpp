#include <iostream>
#include <sstream>
#include <tuple>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utilities.h"
#include "wrappers.h"

using namespace std;

tuple<string, uint16_t> get_ip_port_from_addr_struct(sockaddr_storage& client_address) {
    char ip_holder[INET6_ADDRSTRLEN];  // универсальный буфер, достаточный для хранения как адреса IPv4, так и адреса IPv6
    uint16_t port = 0;

    switch (client_address.ss_family) {
        case AF_INET: {  // IPv4
            auto* addr4 = (struct sockaddr_in *)&client_address;
            Inet_ntop(AF_INET, &addr4->sin_addr, ip_holder, sizeof(ip_holder));
            port = ntohs(addr4->sin_port);  // returns the value in host byte order
            break;
        }
        case AF_INET6: {  // IPv6
            auto* addr6 = (struct sockaddr_in6 *)&client_address;
            Inet_ntop(AF_INET6, &addr6->sin6_addr, ip_holder, sizeof(ip_holder));
            port = ntohs(addr6->sin6_port);  // returns the value in host byte order
            break;
        }
        default:         // Неизвестное семейство
            strcpy(ip_holder, "Unknown family address");
    }

    return tuple{ip_holder, port};
}


void handle_client(int connected_fd, const string& message) {
    size_t msg_len = message.size();
    size_t total_sent = 0;

    size_t count = 0;
    constexpr size_t buffer_size = 1024;
    char buffer[buffer_size];


    while (total_sent < msg_len) {
        size_t len_to_send = std::min(buffer_size, msg_len - total_sent);
        memcpy(buffer, message.c_str() + total_sent, len_to_send);
        ssize_t num_bytes_sent = Send(connected_fd, buffer, len_to_send, 0);
        ++count;
        total_sent += static_cast<size_t>(num_bytes_sent);
    }

    cout << "Function send was called " << count << " times." << endl;
    cout << "Message sent to client.\n" << endl;
}


// проверяет количество ссылок (link count) на файловый дескриптор
void check_link_count(int fd) {
    struct stat stat_buf{};

    if (fstat(fd, &stat_buf) == 0) {
        cout << "Количество ссылок: " << stat_buf.st_nlink << endl;
    } else {
        cerr << "fstat failed: " << strerror(errno) << endl;
    }
}

