/*
 * Эта версия эхо-клиента решает следующую задачу.
 * Клиентский процесс может оказаться заблокированным в вызове функции fgets (читает из STDIN),
 * в то время как на сокете произойдет какое-то событие (например, получение сегмента [FIN]).
 * В этой версии клиента блокирование происходит в функции select в ожидании готовности ЛИБО STDIN, ЛИБО сокета.
 *
 * Сокет обрабатывает 3 условия:
 * 1. Если TCP сервера отправляет данные, сокет становится готовым для чтения, и функция "recv" возвращает
 *    положительное значение, т.е. количество принятых байтов данных.
 * 2. Если TCP сервера отправляет сегмент [FIN] (обрабатывающий процесс сервера завершается), сокет становится
 *    готовым для чтения, и функция "recv" возвращает "0" (признак конца файла).
 * 3. Если TCP сервера отправляет сегмент [RST] (узел вышел из строя и перезагрузился), сокет становится
 *    готовым для чтения, и функкция "recv" возвращает "-1", а переменная "errno" содержит код ошибки.
 */

#include <vector>
#include <sys/socket.h>  // socket(), recv()
#include <netinet/in.h>  // struct sockaddr_in, struct in_addr, htons(...)
#include <thread>
#include <chrono>

#include "wrappers.h"
#include "utilities.h"
#include "parsing.h"

using namespace std;
using namespace chrono_literals;

int main(int argc, char* argv[]) {
    string server_ip{};
    uint16_t server_port{};
    constexpr size_t MAX_CONNECTIONS = 1;
    vector<int> client_sockets{MAX_CONNECTIONS};

    echo_client_check_arguments(argc, argv, server_ip, server_port);

    /*
     * Клиент устанавливает "MAX_CONNECTIONS" соединений, а задем использует первое из них "client_sockets[0]"
     * для вызова функции "client_str_echo". Несколько соединений устанавливаются для того, чтобы породить от
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
    /*client_str_echo(stdin, socket_fd);*/
    client_str_echo(stdin, client_sockets[0]);

    // Явно вызываем close() для завершения соединения на стороне клиента.
    // Это нужно для того, чтобы  освободить все ресурсы, связанные с этим соединением, включая файловые дескрипторы.
    /*Close(socket_fd);*/
//    Close(client_sockets[0]);
    shutdown(client_sockets[0], SHUT_WR);
    std::this_thread::sleep_for(6s);

    // В Linux при завершении процесса ядро автоматически закрывает все открытые файловые дескрипторы, включая сокеты.
    // Если процесс клиента завершился, ядро закроет сокет, и в результате будет отправлен [FIN, ACK].
    return 0;
}
