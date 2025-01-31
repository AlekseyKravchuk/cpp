#include <iostream>
#include <sstream>

#include "utilities.h"
#include "wrappers.h"

using namespace std;

string info_about_connected_client(sockaddr_storage& client_address) {
    char buf[INET6_ADDRSTRLEN];  // универсальный буфер, достаточный для хранения как адреса IPv4, так и адреса IPv6
    uint16_t port = 0;

    switch (client_address.ss_family) {
        case AF_INET: {  // IPv4
            auto* addr4 = (struct sockaddr_in *)&client_address;
            Inet_ntop(AF_INET, &addr4->sin_addr, buf, sizeof(buf));
            port = ntohs(addr4->sin_port);  // returns the value in host byte order
            break;
        }
        case AF_INET6: {  // IPv6
            auto* addr6 = (struct sockaddr_in6 *)&client_address;
            Inet_ntop(AF_INET6, &addr6->sin6_addr, buf, sizeof(buf));
            port = ntohs(addr6->sin6_port);  // returns the value in host byte order
            break;
        }
        default:         // Неизвестное семейство
            strcpy(buf, "Unknown family address");
    }

    ostringstream oss;
    oss << "Connected client: [IP = " << buf << "], "
        << "[PORT = " << port << "]" << endl;

    return oss.str();
}
