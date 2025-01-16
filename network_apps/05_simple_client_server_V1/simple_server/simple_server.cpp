/*
 * Simple TCP-server that listen on user-defined socket
 */

#include <iostream>

#include <sys/socket.h>  // socket(...), recv(...)

#include <cstdlib>       // exit(...)
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...), INET_ADDRSTRLEN
#include <arpa/inet.h>   // inet_pton(...)
#include <cstdio>
#include <unistd.h>      // close(...)

#include <string>
#include <cstring>       // std::strerror - analogue of std::perror()

#include <thread>
#include <chrono>

#include "wrappers.h"

using namespace std;
using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
    int server_fd, client_fd;
    uint16_t port_listen_to = 0;
    const int MAX_QUEUE_PENDING_CONNECTIONS_LEN = 10;
    // ===========================================================================

    constexpr size_t buffer_size = 256;
    char buffer[buffer_size];
    const char* message = "Война и мир, Лев Толстой (фрагмент)\n\n"
                          "В первый раз война, как мы знаем, началась, но она была странной. "
                          "Скоро стало очевидно, что она не только заставит нас бороться за нашу жизнь, "
                          "но и откроет нам новые горизонты. Наша страна встретила войну, как испытание. "
                          "Никто не мог точно предсказать, как она повлияет на нас, но каждый был уверен, что "
                          "эта борьба будет долгой и трудной. Этот момент был одним из тех, когда жизнь человечества "
                          "поменяла свою траекторию. Все, кто принял участие, поняли это в тот момент, когда началась война."
                          "В первый раз война, как мы знаем, началась, но она была странной. "
                          "Скоро стало очевидно, что она не только заставит нас бороться за нашу жизнь, "
                          "но и откроет нам новые горизонты. Наша страна встретила войну, как испытание. "
                          "Никто не мог точно предсказать, как она повлияет на нас, но каждый был уверен, что "
                          "эта борьба будет долгой и трудной. Этот момент был одним из тех, когда жизнь человечества "
                          "поменяла свою траекторию. Все, кто принял участие, поняли это в тот момент, когда началась война.";

    if (argc != 2) {
        cerr << "usage: " << argv[0] << " <port_listen_to>" << endl;
        perror("wrong number of arguments");
        exit(EXIT_FAILURE);
    } else {
        port_listen_to = static_cast<uint16_t>(std::stoul(argv[1]));
    }

    // ====== Создаем серверный сокет (Listening Socket) =====
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Server socket error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    // ======== Настраиваем структуру адреса сервера =========
    struct sockaddr_in server_address = {
            .sin_family = AF_INET,
            .sin_port = htons(port_listen_to),
            .sin_addr = {INADDR_ANY},
            .sin_zero = {}
    };
    // =======================================================

    // ========== Привязываем сокет к адресу и порту =========
    if (bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        cerr << "Socket bind error: " << strerror(errno) << std::endl;
        if (server_fd > 0) {
            close(server_fd);
        }
        exit(EXIT_FAILURE);
    }

    // =============== Ожидание входящих соединений на прослушиваемом порту ===============
    if ((listen(server_fd, MAX_QUEUE_PENDING_CONNECTIONS_LEN)) < 0) {
        cerr << "Listen error on server socket: " << strerror(errno) << endl;
        if (server_fd > 0) {
            close(server_fd);
        }
        exit(EXIT_FAILURE);
    }
    cout << "Server is waiting connection on port " << port_listen_to << "..." << endl;

    // =============== Принятие соединения с клиентом ===============
    // Создаем структуру "sockaddr_in" для хранения информации о клиенте.
    sockaddr_in client_address{};
    socklen_t client_address_len = sizeof(client_address);

    client_fd = accept(server_fd,
                       (struct sockaddr*) &client_address,
                       &client_address_len);
    if (client_fd < 0) {
        cerr << "Accept connection error: " << strerror(errno) << endl;
        if (server_fd > 0) {
            close(server_fd);
        }
        exit(EXIT_FAILURE);
    }

    // =============== Вывод информации о подключении клиента ===============
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
    cout << "Connected client ==> " << client_ip << ":" << ntohs(client_address.sin_port) << endl << endl;


    // =============== Отправка данных клиенту ===============
    size_t message_len = strlen(message);
    size_t total_sent = 0;

    size_t count = 0;

    while (total_sent < message_len) {
        size_t len_to_send = std::min(buffer_size, message_len - total_sent);
        memcpy(buffer, message + total_sent, len_to_send);
        ssize_t num_bytes_sent = Send(client_fd, buffer, len_to_send, 0);

//        ssize_t num_bytes_sent = send(client_fd, buffer, len_to_send, 0);
//        if (num_bytes_sent < 0) {
//            cerr << "Sending data error (send call): " << strerror(errno) << endl;
//            exit(EXIT_FAILURE);
//        }

        ++count;



        total_sent += static_cast<size_t>(num_bytes_sent);
    }

    cout << "Function send was called " << count << " times." << endl;
    cout << "Message sent to client." << endl;

    std::this_thread::sleep_for(10s); // 10 секунд

    // Первым закрывается сокет клиента (client_fd), т.к. этот сокет участвует в процессе обмена данными с клиентом,
    // и его нужно закрыть сразу после завершения общения с клиентом.
    // После этого сокет становится бесполезным, и его необходимо закрыть, чтобы освободить ресурсы.
    if (client_fd > 0) {
        close(client_fd);
    }

    // Закрытие сокета сервера (server_fd) происходит после того, как ВСЕ соединения с клиентами завершены,
    // то есть после того, как все клиентские сокеты были закрыты:
    // server_fd продолжает слушать входящие соединения, даже после того, как вы закрыли client_fd.
    // Если по какой-то причине сервер закрывает свой сокет раньше, чем завершается обработка клиентских соединений,
    // он больше не сможет принимать новые подключения.
    if (server_fd > 0) {
        close(server_fd);
    }

    return 0;
}
