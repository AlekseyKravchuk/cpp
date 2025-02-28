#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utilities.h"
#include "wrappers.h"

#define MAX_BUF_SIZE 4096  /* Максимальный размер строки (буфера, в который осуществляется чтение из сокета) */

using namespace std;

// ############################## Server-related ##############################
// очищает таблицу завершенных дочерних процессов (обработка зомбированных процессов)
void sigchld_handler(int signal_number) {
    pid_t pid;
    int stat;

    // Значение для pid = -1 означает, что нужно дождаться завершения первого дочернего процесса

    // Освобождаем ресурсы завершенных процессов
    /*
     * WNOHANG: если дочерний процесс еще не завершился, то waitpid сразу вернет 0, не блокируя выполнение программы.
     * Это полезно в асинхронных сценариях, когда мы не хотим блокировать родительский процесс на ожидание завершения.
     * Без WNOHANG waitpid будет блокировать выполнение программы до тех пор, пока хотя бы один дочерний процесс не завершится.
     */
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        cout << "sigchld_handler: child with PID=" << pid << " terminated" << endl;
    }
}

void set_signal_handler() {
    // sigaction — это структура, используемая в современном POSIX API для работы с сигналами.
    // Она позволяет задать обработчик сигнала и его параметры.
    struct sigaction sig_act{};

    // sig_act.sa_handler - это указатель на функцию, которая будет вызвана при получении сигнала SIGCHLD
    sig_act.sa_handler = sigchld_handler;

    // sigemptyset(&sa.sa_mask) очищает маску (никакие сигналы не блокируются дополнительно).
    // sa_mask определяет набор сигналов, которые будут временно заблокированы во время выполнения обработчика.
    sigemptyset(&sig_act.sa_mask);

    // Установка флагов обработчика
    // SA_RESTART:   - Автоматически перезапускает прерванные системные вызовы
    //               (accept(), read(), write(), recv(), send() и т. д.), если они были прерваны сигналом.
    //               - Без него системные вызовы могут завершаться с EINTR, что приведет к accept()=-1 (ошибка на сервере).
    // SA_NOCLDSTOP: - Предотвращает ненужные сигналы
    //               - Означает, что SIGCHLD не будет приходить, если дочерний процесс
    //                 просто приостановлен (SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU).
    //               - Без него сервер мог бы получать SIGCHLD не только при завершении процессов,
    //                 но и при их остановке (например, если кто-то отправил SIGSTOP дочернему процессу).
    sig_act.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sig_act, nullptr) == -1) {
        cerr << "sigaction error: " << strerror(errno) << endl;
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

void server_str_echo(int sock_fd) {
    ssize_t n;
    char line[MAX_BUF_SIZE];

    while (true) {
        /*n = recv(sock_fd, line, MAX_BUF_SIZE, 0);  // Получаем "n" байт из сетевого сокета "sock_fd" в буфер "line"*/
        n = readline(sock_fd, line, MAX_BUF_SIZE);   // Получаем "n" байт из сетевого сокета "sock_fd" в буфер "line"

        if (n > 0) {
            vector<int64_t> args;
            char* p = line;
            for (;;) {
                errno = 0;
                char* p_end = nullptr;
                int64_t value = std::strtol(p, &p_end, 10);

                if (p == p_end) {
                    break;
                }

                if (errno == ERANGE) {
                    cerr << "std::strtol: range error occured.\n";
                }

                p = p_end;
                args.push_back(value);
            }

            ostringstream oss;
            if (args.size() == 2) {
                oss << args[0] + args[1] << endl;
            } else {
                oss << "input error" << endl;
            }

            const string& out_str = oss.str();
            n = static_cast<ssize_t>(strlen(out_str.c_str()));
            Write_n_bytes_to_sock_fd(sock_fd, out_str.c_str(), n);
        } else if (n < 0 && errno == EINTR) {
            continue; // Чтение было прервано сигналом; повторяем попытку
        } else if (n < 0) {
            cerr << "str_echo: read error" << endl;
            break;
        } else {
            break; // n == 0, клиент закрыл соединение
        }
    }
}

// ############################## Client-related ##############################
void print_info_about_connected_client(sockaddr_storage& client_address) {
    auto [client_ip, client_port] = get_ip_port_from_addr_struct(client_address);
    cout << "Connected client: [IP = " << client_ip << "], " << "[PORT = " << client_port << "]" << endl;
}

void print_client_info(const string& server_ip, uint16_t server_port, int socket_fd) {
    sockaddr_storage client_struct_address{};  // используем универсальную структуру адреса для адреса подключенного клиента
    socklen_t addr_len = sizeof(client_struct_address);

    // "getsockname" вызывается ПОСЛЕ установления соединения (после успешного завершения функции "connect")
    // и возвращает IP адрес и номер локального порта, присвоенные ядром ОС.
    getsockname(socket_fd, (struct sockaddr*) &client_struct_address, &addr_len);

    auto [client_ip, client_port] = get_ip_port_from_addr_struct(client_struct_address);

    std::cout << "Client " << client_ip << ":" << client_port
              << " (PID = " << getpid() << ") "
              << "is connected to server " << server_ip << ":" << server_port << endl;
}

void client_str_echo(FILE* stdin_file, int sock_fd) {
    char send_buf[MAX_BUF_SIZE];
    char recv_buf[MAX_BUF_SIZE];

    fd_set read_fds;  // нам нужен только 1 набор дескрипторов - для проверки готовности сокета для чтения
    FD_ZERO(&read_fds);
    int file_fd = fileno(stdin_file);
    int max_fd = std::max(file_fd, sock_fd) + 1; // номер наибольшего дескриптора + 1

    for (;;) {
        // если использовать select() в цикле, наборы должны быть повторно инициализированы перед каждым вызовом.
        FD_SET(file_fd, &read_fds);
        FD_SET(sock_fd, &read_fds);

        // "nfds" должен быть на 1 больше, чем наибольший файловый дескриптор в множествах fd_set
        Select(max_fd, &read_fds, nullptr, nullptr, nullptr);

        if (FD_ISSET(sock_fd, &read_fds)) {	     // сокет готов для чтения
            if (Readline(sock_fd, recv_buf, MAX_BUF_SIZE) == 0) {
                cerr << "str_cli: server terminated prematurely" << endl;
            }

            // Записываем в stdout строку, полученную из сокета и хранимую в буфере "recv_buf"
            Fputs(recv_buf, stdout);
        }

        if (FD_ISSET(file_fd, &read_fds)) {       // STDIN готов для чтения
            if (Fgets(send_buf, MAX_BUF_SIZE, stdin_file) == nullptr) {
                return;		/* all done */
            }
            Write_n_bytes_to_sock_fd(sock_fd, send_buf, strlen(send_buf));
        }
    }
}

/*
 * sock_fd         - дескриптор сетевого сокета, из которого читаем
 * ptr_to_recv_buf - указатель на буфер, в который записываем полученные данные
 * max_len         - максимальный размер буфера
 */
ssize_t readline(int sock_fd, void* ptr_to_recv_buf, size_t max_len) {
    ssize_t w_count;          // счетчик записанных байтов в буфер, на который указывает "ptr_to_recv_buf"
    ssize_t res;              // возвращаемое значение из фукнции "my_read"
    char ch;                  // временный буфер для одного считанного функцией "my_read" символа
    char* pos_ptr = nullptr;  // указатель на текущую позицию в буфере, на который указывает "ptr_to_recv_buf"

    pos_ptr = reinterpret_cast<char* >(ptr_to_recv_buf);

    for (w_count = 1; w_count < max_len; ++w_count) {
        if ((res = my_read(sock_fd, &ch)) == 1) {
            *pos_ptr++ = ch;
            if (ch == '\n')
                break;             // newline is stored, like fgets()
        } else if (res == 0) {
            *pos_ptr = 0;
            return (w_count - 1);  // EOF, w_count - 1 bytes were read */
        } else
            return (-1);           // error, errno set by read() */
    }

    *pos_ptr = 0;                  /* null terminate like fgets() */

    return w_count;
}

// my_read реализует идею буферизации + построчного чтения:
// ===== буферизированное построчное чтение функцией my_read =====
//     - Читает сразу большой блок через read (эквивалент recv без MSG_PEEK).
//     - Хранит данные в read_buf и выдаёт по 1 байту из буфера.
//     - Если встретился \n, readline завершает чтение строки.
// ch_ptr - это адрес, куда "my_read" должна записать 1 байт
ssize_t my_read(int sock_fd, char* ch_ptr) {
    static ssize_t n_bytes_read = 0;
    static char read_buf[MAX_BUF_SIZE];
    static char* read_ptr = nullptr;

    // при первом вызове "my_read" мы "проваливаемся" в цикл "while"
    while (n_bytes_read <= 0) {
        // считываем из сетевого сокета "sock_fd" полученные байты в буфер "read_buf" (максимум sizeof(read_buf)) байт
        n_bytes_read = recv(sock_fd, read_buf, sizeof(read_buf), 0);

        if (n_bytes_read < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        } else if (n_bytes_read == 0) {
            cout << "my_read: n_bytes_read = 0 ===> quit" << endl;
            return 0;
        }
        read_ptr = read_buf;
    }

    n_bytes_read--;
    *ch_ptr = *read_ptr++;

    return 1;
}

// ############################# Common functions #############################
/* Write "n" bytes starting from "buf_start" to a socket descriptor "sock_fd". */
ssize_t write_n_bytes_to_sock_fd(int sock_fd, const void* buf_start, size_t n) {
    size_t n_left;
    ssize_t n_written;
    const char* ptr;

    ptr = reinterpret_cast<const char*>(buf_start);
    n_left = n;

    while (n_left > 0) {
        if ((n_written = Send(sock_fd, ptr, n_left, 0)) <= 0) {
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

// ========================================================================================
int Select(int n_fds, fd_set* read_fds, fd_set* write_fds, fd_set* except_fds, struct timeval* timeout) {
    int n;

    if ((n = select(n_fds, read_fds, write_fds, except_fds, timeout)) < 0) {
        cerr << "select error" << endl;
    }

    // Возвращаемое значение м.б. равно нулю, если тайм-аут истек до того, как какие-либо файловые дескрипторы стали готовы.
    return (n);
}
