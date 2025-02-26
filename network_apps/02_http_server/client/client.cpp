#include <iostream>
#include <vector>
#include <sys/socket.h>  // socket(), recv()
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)

#include "wrappers.h"
#include "utilities.h"
#include "parsing.h"

using namespace std;

int main(int argc, char* argv[]) {
    string server_ip{};
    uint16_t server_port{};
    constexpr size_t MAX_CONNECTIONS = 1;
    vector<int> client_sockets{MAX_CONNECTIONS};

    echo_client_check_arguments(argc, argv, server_ip, server_port);

    /*
     * Клиент устанавливает "MAX_CONNECTIONS" соединений, а задем использует первое из них "client_sockets[0]"
     * для вызова функции "send_http_request". Несколько соединений устанавливаются для того, чтобы породить от
     * concurrent-сервера множество дочерних процессов
    */
    for (size_t i = 0; i < MAX_CONNECTIONS; ++i) {
        client_sockets[i] = Socket(AF_INET, SOCK_STREAM, 0);

        // Частично заполняем server_address типа "sockaddr_in":
        sockaddr_in server_address{
                .sin_family = AF_INET,
                .sin_port = htons(server_port),
                .sin_addr = {},
                .sin_zero = {}
        };

        // Преобразование IP-адреса из точечно-десятичной нотации в двоичный вид (network-byte order)
        Inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr);

        Connect(client_sockets[i], (sockaddr*) &server_address, sizeof(server_address));
        print_client_info(server_ip, server_port, client_sockets[i]);
    }

    // Эта функция выполняет все необходимые действия со стороны клиента.
    /*send_http_request(stdin, socket_fd);*/
    send_http_request(stdin, client_sockets[0]);

//    char response[4096] = {'\0'}; // инициализируем буфер нулями
//    ssize_t bytes_count = recv(client_sockets[0], &response, 4096, 0);
//    if (bytes_count > 0) {
//        response[bytes_count] = '\0'; // Гарантированно завершаем строку
//        cout << "Client got response from server: " << response << endl;
//    } else if (bytes_count == 0) {
//        cout << "Server closed the connection gracefully.\n";
//    } else {
//        cerr << "recv failed: " << ::strerror(errno) << endl;
//    }

//    string response(4096, '\0'); // Выделяем инициализированных '\0' 4096 байт памяти под строку
//    ssize_t bytes_count = recv(client_sockets[0], &response[0], response.size(), 0);
//    if (bytes_count > 0) {
//        response.resize(static_cast<size_t>(bytes_count));  // Обрезаем строку до реального количества полученных данных
//        cout << "Client got response from server: " << response << endl;
//    } else if (bytes_count == 0) {
//        cout << "Server closed the connection gracefully.\n";
//    } else {
//        cerr << "recv failed: " << ::strerror(errno) << endl;
//    }

    vector response(4096, '\0'); // Выделяем инициализированных '\0' 4096 байт памяти в векторе
    ssize_t bytes_count = recv(client_sockets[0], response.data(), response.size(), 0);
    if (bytes_count > 0) {
        response.resize(static_cast<size_t>(bytes_count));  // Обрезаем строку до реального количества полученных данных
        cout << "Client got response from server: " << string(response.data(), bytes_count) << endl;
    } else if (bytes_count == 0) {
        cout << "Server closed the connection gracefully.\n";
    } else {
        cerr << "recv failed: " << ::strerror(errno) << endl;
    }


    // Явно вызываем close() для завершения соединения на стороне клиента.
    // Это нужно для того, чтобы  освободить все ресурсы, связанные с этим соединением, включая файловые дескрипторы.
    /*Close(socket_fd);*/
    Close(client_sockets[0]);

    // В Linux при завершении процесса ядро автоматически закрывает все открытые файловые дескрипторы, включая сокеты.
    // Если процесс клиента завершился, ядро закроет сокет, и в результате будет отправлен [FIN, ACK].
    return 0;
}
