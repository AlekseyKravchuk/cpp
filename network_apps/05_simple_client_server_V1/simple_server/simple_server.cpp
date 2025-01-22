/*
 * Simple TCP-server that listen on user-defined socket
 */

#include <iostream>
#include <sys/socket.h>  // socket(...), recv(...)
#include <cstdlib>       // exit(...)
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...), INET_ADDRSTRLEN
#include <arpa/inet.h>   // inet_pton(...)
#include <string>
#include <cstring>       // std::strerror - analogue of std::perror()
#include <thread>
#include <chrono>
#include <boost/program_options.hpp>

#include "wrappers.h"

using namespace std;
using namespace std::chrono_literals;
namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    int listen_fd, client_fd;
    uint16_t port_listen_to = 0;
    constexpr int MAX_QUEUE_PENDING_CONNECTIONS_LEN = 10;
    string file_path, message;
    // ===========================================================================

    server_check_arguments(argc, argv, port_listen_to, file_path);

    try {
        message = get_content(file_path);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    // ====== Создаем сокет для приема запросов на соединение (listening socket) =====
    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

    // ======== Настраиваем структуру адреса сервера =========
    struct sockaddr_in server_address = {
            .sin_family = AF_INET,
            .sin_port = htons(port_listen_to),
            .sin_addr = {INADDR_ANY},
            .sin_zero = {}
    };

    // ========== Привязываем сокет к адресу и порту =========
    Bind(listen_fd, (struct sockaddr*)&server_address, sizeof(server_address));

    // =============== Ожидание входящих соединений на прослушиваемом порту ===============
    Listen(listen_fd, MAX_QUEUE_PENDING_CONNECTIONS_LEN);
    cout << "Server (PID = " << getpid() << ") is waiting connection on port " << port_listen_to << "..." << endl;

    // =============== Принятие соединения с клиентом ===============
    // Создаем структуру "sockaddr_in" для хранения информации о клиенте.
    sockaddr_in client_address{};
    socklen_t client_address_len = sizeof(client_address);

//    // TODO: 1) сделать так, чтобы сервер мог обрабатывать множество клиентов, а не только одного;
//             2) завернуть клиентов и сервер в сеть на основе docker;
//             3)
    client_fd = Accept(listen_fd, (struct sockaddr*) &client_address, &client_address_len);

    // =============== Вывод информации о подключении клиента ===============
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
    cout << "Connected client ==> " << client_ip
         << ":" << ntohs(client_address.sin_port) << endl << endl;

    // std::this_thread::sleep_for(30s); // 30 секунд

    // =============== Отправка данных подключившемуся клиенту ===============
    size_t message_len = message.size();
    size_t total_sent = 0;

    size_t count = 0;
    constexpr size_t buffer_size = 1024;
    char buffer[buffer_size];
    
    while (total_sent < message_len) {
        size_t len_to_send = std::min(buffer_size, message_len - total_sent);
        memcpy(buffer, message.c_str() + total_sent, len_to_send);
        ssize_t num_bytes_sent = Send(client_fd, buffer, len_to_send, 0);
        ++count;
        total_sent += static_cast<size_t>(num_bytes_sent);
    }

    cout << "Function send was called " << count << " times." << endl;
    cout << "Message sent to client." << endl;

    // Первым закрывается сокет клиента (client_fd), т.к. этот сокет участвует в процессе обмена данными с клиентом,
    // и его нужно закрыть сразу после завершения общения с клиентом.
    // После этого сокет становится бесполезным, и его необходимо закрыть, чтобы освободить ресурсы.
    Close(client_fd);

    // Закрытие сокета сервера (listen_fd) происходит после того, как ВСЕ соединения с клиентами завершены,
    // то есть после того, как все клиентские сокеты были закрыты:
    // listen_fd продолжает слушать входящие соединения, даже после того, как вы закрыли client_fd.
    // Если по какой-то причине сервер закрывает свой сокет раньше, чем завершается обработка клиентских соединений,
    // он больше не сможет принимать новые подключения.
    Close(listen_fd);

    return 0;
}
