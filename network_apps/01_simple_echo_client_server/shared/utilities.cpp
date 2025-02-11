#include <iostream>
#include <tuple>
#include <sys/types.h>
#include <unistd.h>
#include <filesystem>

#include <boost/program_options.hpp>

#include "utilities.h"
#include "wrappers.h"

#define MAX_BUF_SIZE 4096  /* Максимальный размер строки (буфера, в который осуществляется чтение из сокета) */

using namespace std;
namespace po = boost::program_options;
namespace fs = std::filesystem;

void echo_client_check_arguments(int argc,
                                 char* argv[],
                                 string& server_ip,
                                 uint16_t& server_port) {
    // Declare the supported options using the options_description class.
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("ipaddr,i", po::value<string>()->required(), "server IP address to connect to")
            ("port,p", po::value<uint16_t>()->required(), "server port to connect to");

    // переменная для хранения значений опций
    po::variables_map vars_map;

    // парсинг аргументов
    try {
        po::store(po::parse_command_line(argc, argv, desc), vars_map);

        if (vars_map.count("help")) {
            std::cout << desc;
            exit(EXIT_SUCCESS);
        }

        // Вызов notify предназначен для обработки обязательных аргументов;
        // проверяет, корректно ли заполнены значения опций в vars_map (объект po::variables_map)
        // и вызывает обработчики (notifiers), если они были заданы для опций.
        po::notify(vars_map);

        // Присваиваем значения из командной строки в переменные
        server_ip = vars_map["ipaddr"].as<string>();
        server_port = vars_map["port"].as<uint16_t>();
    } catch (const po::error& e) {
        // Если обязательная опция отсутствует, выводим пользовательское сообщение
        std::cerr << "Error: Missing required option: " << e.what() << "\n";

        fs::path full_path = fs::absolute(argv[0]); // Получаем абсолютный путь
        fs::path relative_path = fs::relative(full_path, fs::current_path()); // Преобразуем в относительный

        std::cout << "usage: " << relative_path << " <server_ip> <port>\n";
        desc.print(std::cerr);
        exit(EXIT_FAILURE);
    } catch (const std::exception& e) {  // Ловим другие возможные ошибки
        std::cerr << "Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }
}

void echo_server_check_arguments(int argc,
                                 char* argv[],
                                 uint16_t& port_listen_to) {
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("port,p", po::value<uint16_t>()->required(), "port to listen on");

    po::variables_map vars_map;

    try {
        po::store(po::parse_command_line(argc, argv, desc), vars_map);

        if (vars_map.count("help")) {
            std::cout << desc << "\n";
            exit(EXIT_SUCCESS);;
        }

        // Вызов notify для обработки обязательных аргументов
        po::notify(vars_map);

        // Присваиваем значения из командной строки в переменные
        port_listen_to = vars_map["port"].as<uint16_t>();

        std::cout << "Command args successfully parsed, port to listen to (server side): " << port_listen_to << "\n";
    } catch (const po::error& e) {
        // Если обязательная опция отсутствует, выводим пользовательское сообщение
        fs::path full_path = fs::absolute(argv[0]); // Получаем абсолютный путь
        fs::path relative_path = fs::relative(full_path, fs::current_path()); // Преобразуем в относительный

        std::cerr << "Error: Missing required option: " << e.what() << "\n";
        std::cout << "usage: " << relative_path << " <port_listen_to> <file_to_read_text_from>\n";
        desc.print(std::cerr);
        exit(EXIT_FAILURE);
    } catch (const std::exception& e) {
        // Ловим другие возможные ошибки
        std::cerr << "Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }
}

tuple<string, uint16_t> get_ip_port_from_addr_struct(sockaddr_storage& client_address) {
    char ip_holder[INET6_ADDRSTRLEN];  // универсальный буфер, достаточный для хранения как адреса IPv4, так и адреса IPv6
    uint16_t port = 0;

    switch (client_address.ss_family) {
        case AF_INET: {  // IPv4
            auto* addr4 = (struct sockaddr_in*) &client_address;
            Inet_ntop(AF_INET, &addr4->sin_addr, ip_holder, sizeof(ip_holder));
            port = ntohs(addr4->sin_port);  // returns the value in host byte order
            break;
        }
        case AF_INET6: {  // IPv6
            auto* addr6 = (struct sockaddr_in6*) &client_address;
            Inet_ntop(AF_INET6, &addr6->sin6_addr, ip_holder, sizeof(ip_holder));
            port = ntohs(addr6->sin6_port);  // returns the value in host byte order
            break;
        }
        default:         // Неизвестное семейство
            strcpy(ip_holder, "Unknown family address");
    }

    return tuple{ip_holder, port};
}

/* Write "n" bytes to a descriptor "fd". */
ssize_t write_n_bytes_to_fd(int fd, const void* vptr, size_t n) {
    size_t n_left;
    ssize_t n_written;
    const char* ptr;

    ptr = reinterpret_cast<const char*>(vptr);
    n_left = n;

    while (n_left > 0) {
        if ((n_written = send(fd, ptr, n_left, 0)) <= 0) {
            if (n_written < 0 && errno == EINTR)
                n_written = 0;        /* and call write() again */
            else
                return (-1);          /* error */
        }

        n_left -= n_written;
        ptr += n_written;
    }

    return static_cast<ssize_t>(n);
}

void server_str_echo(int sock_fd) {
    ssize_t n_bytes_read;
    char buf[MAX_BUF_SIZE];
    bool should_retry = false;

    while (true) {
        // Считываем данные из сокета "sock_fd" в буфер "buf"
        n_bytes_read = recv(sock_fd, buf, MAX_BUF_SIZE, 0);

        if (n_bytes_read > 0) {
            cout << "Успешно прочитано из сокета " << n_bytes_read << " байт.\n"
                 << "Содержимое: " << buf << endl;
            // Успешное чтение, записываем данные ("n_bytes_read" байтов) обратно в сетевой сокет
            Write_n_bytes_to_sock_fd(sock_fd, buf, n_bytes_read);
        } else if (n_bytes_read < 0 && errno == EINTR) {
            // =========== Чтение было прервано сигналом; повторяем попытку ===========
            /*EINTR (Interrupted system call) возникает, когда системный вызов прерывается сигналом.
             * Это стандартная ошибка в Unix-подобных системах, которая указывает на то, что системный вызов,
             * такой как read, write, или accept, был прерван сигналом и не был выполнен полностью.*/
            should_retry = true;
        } else if (n_bytes_read < 0) {
            cerr << "server_str_echo: read error: " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }

        // Если чтение НЕ было прервано сигналом, выходим из цикла, иначе продолжаем крутиться в цикле
        if (!should_retry) {
            break;
        }

        should_retry = false;  // Сброс флага
    }
}

ssize_t my_read(int fd, char* ptr) {
    static ssize_t read_cnt;
    static char* read_ptr;
    static char read_buf[MAX_BUF_SIZE];

    while (read_cnt <= 0) {
        read_cnt = read(fd, read_buf, sizeof(read_buf));
        if (read_cnt < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        } else if (read_cnt == 0) {
            return 0;
        }
        read_ptr = read_buf;
    }

    read_cnt--;
    *ptr = *read_ptr++;
    return EXIT_FAILURE;
}

ssize_t readline(int fd, void* vptr, size_t max_len) {
    ssize_t n;
    ssize_t rc;
    char c;
    char* ptr;

    ptr = reinterpret_cast<char* >(vptr);

    for (n = 1; n < max_len; n++) {
        if ((rc = my_read(fd, &c)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;    /* newline is stored, like fgets() */
        } else if (rc == 0) {
            *ptr = 0;
            return (n - 1);    /* EOF, n - 1 bytes were read */
        } else
            return (-1);        /* error, errno set by read() */
    }

    *ptr = 0;    /* null terminate like fgets() */
    return (n);
}

void client_str_echo(FILE* fp, int sock_fd) {
    char send_buf[MAX_BUF_SIZE];
    char recv_buf[MAX_BUF_SIZE];

    /*
     * char *fgets(char* str, int size, FILE* stream);
     * стандартная функция из <stdio.h> в POSIX-системах, предназначенная для безопасного чтения строк из потока.
     * char *str (send_buf)     — буфер, в который записывается считанная строка;
     * int size  (MAX_BUF_SIZE) — максимальное количество символов, которые можно записать (включая завершающий \0);
     * FILE *stream (fp)        — указатель на поток (stdin, fd из fopen() и т.д), ИЗ КОТОРОГО считываются данные.
     * ================================================
     * Возвращаемое значение:
     *  - Указатель на str при успешном чтении.
     *  - NULL, если произошла ошибка или достигнут конец файла (EOF).
     * ================================================
     * Особенности:
     * fgets() читает строку, пока не встретит символ \n (перенос строки) или пока не будет прочитано (size - 1) символов.
     * fgets() читает не более (size-1) символов, т.к. она гарантированно добавляет завершающий нулевой символ (\0) в конец строки.
     * Завершает строку \0, даже если \n не встречен.
     * Если строка длиннее, чем size - 1, то fgets() читает только часть, а остальное останется в потоке.
     * В отличие от gets() (которая небезопасна и удалена из C11), fgets() предотвращает переполнение буфера.
     */
    while (Fgets(send_buf, MAX_BUF_SIZE, fp) != nullptr) {

        // "Write_n_bytes_to_sock_fd" отправляет считанную функцией "Fgets" строку серверу.
        Write_n_bytes_to_sock_fd(sock_fd, send_buf, strlen(send_buf));

        if (Readline(sock_fd, recv_buf, MAX_BUF_SIZE) == 0) {
            cerr << "client_str_echo: server terminated prematurely: " << endl;
            exit(EXIT_FAILURE);
        }

        Fputs(recv_buf, stdout);
    }
}





