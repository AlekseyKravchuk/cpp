#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utilities.h"
#include "wrappers.h"

//#define MAX_BUF_SIZE 4'096  // Максимальный размер буфера, в который осуществляется чтение из сокета за одно чтение
#define MAX_BUF_SIZE 66'000  // Максимальный размер буфера, в который осуществляется чтение из сокета за одно чтение
#define MAX_EVENTS   1'000  // Максимальное количество событий, возвращаемых одним вызовом epoll_wait()

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
        n = readline(sock_fd, line, MAX_BUF_SIZE);   // С буферизацией: получаем "n" байт из сетевого сокета "sock_fd" в буфер "line"

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
    char buf[MAX_BUF_SIZE];
    int max_fd = 0;

    fd_set read_fds;  // нам нужен только 1 набор дескрипторов - для проверки готовности дескриптора для чтения
    FD_ZERO(&read_fds);

    int file_fd = fileno(stdin_file);
    int stdin_eof = 0;  // пока этот флаг равен '0', будем проверять готовность "stdin" к чтению с помощью "select"
    ssize_t n_bytes = 0;

    for (;;) {
        // если использовать select() в цикле, наборы должны быть повторно инициализированы перед каждым вызовом,
        // поскольку каждый вызов select() требует пересоздания множества "fd_set"
        if (stdin_eof == 0) {
            FD_SET(file_fd, &read_fds);
        }

        FD_SET(sock_fd, &read_fds);
        max_fd = std::max(file_fd, sock_fd) + 1; // номер наибольшего дескриптора + 1

        // "nfds" должен быть на 1 больше, чем наибольший файловый дескриптор в множествах fd_set
        Select(max_fd, &read_fds, nullptr, nullptr, nullptr);

        if (FD_ISSET(sock_fd, &read_fds)) {	     //  пришли данные от сервера: сокет готов для чтения
            if ((n_bytes = Recv(sock_fd, buf, MAX_BUF_SIZE, 0)) == 0) {
                if (stdin_eof == 1) {
                    return;  // нормальное завершение
                } else {
                    cerr << "str_cli: server terminated prematurely" << endl;
                    exit(EXIT_FAILURE);
                }
            }
            cout << n_bytes << " bytes was read from socket\n";

            Write(fileno(stdout), buf, n_bytes);
        }

        if (FD_ISSET(file_fd, &read_fds)) {       // STDIN готов для чтения (есть данные на входе)
            if ((n_bytes = Read(file_fd, buf, MAX_BUF_SIZE)) == 0) {
                stdin_eof = 1;
                Shutdown(sock_fd, SHUT_WR); /* send FIN */
                FD_CLR(file_fd, &read_fds);
                continue;
            }
            cout << n_bytes << " bytes was read from file or stdin\n";

            Write_n_bytes_to_sock_fd(sock_fd, buf, n_bytes);
        }
    }
}

void udp_client_echo(FILE* fp, int sock_fd, const struct sockaddr* serv_addr, socklen_t serv_addr_len) {
    char send_buf[MAX_BUF_SIZE];
    char recv_buf[MAX_BUF_SIZE + 1];
    auto p_repply_addr = std::make_unique<sockaddr_storage>();
    socklen_t repply_addr_len = sizeof(*p_repply_addr.get());

    while (Fgets(send_buf, MAX_BUF_SIZE, fp) != nullptr) {
        Sendto(sock_fd, send_buf, strlen(send_buf), 0, serv_addr, serv_addr_len);

        // TODO: сделать так, чтобы клиент не блокировался в recvfrom, если сервер по какой-то причине не отвечает.
        ssize_t n = Recvfrom(sock_fd, recv_buf, MAX_BUF_SIZE, 0, (sockaddr*) p_repply_addr.get(), &repply_addr_len);

        auto [srv_ip, srv_port] = get_ip_port_from_addr_struct(*p_repply_addr.get());
        if (serv_addr_len != repply_addr_len
            || memcmp(serv_addr, p_repply_addr.get(), serv_addr_len) != 0) {
            cout << "reply from ignored [IP:PORT]: " << srv_ip << ":" << srv_port << endl;
            continue;
        } else {
            cout << "UDP reply got from server at: " << srv_ip << ":" << srv_port << endl;
        }

        recv_buf[n] = 0;  // null terminate
        Fputs(recv_buf, stdout);
    }
}

void client_str_echo_using_epoll(FILE* fp, int sock_fd) {
    char buf[MAX_BUF_SIZE];
    int file_fd = fileno(fp);

    ssize_t n_bytes = 0;
    bool eof_reached = false;

    // Инициализируем контекст опроса событий - создаем экземпляр epoll, который будет отслеживать файловые дескрипторы.
    int epoll_fd = Epoll_create1(EPOLL_CLOEXEC);
    // Буфер "events.data()" используется для возвращения из "epoll_wait" информации из списка дескрипторов, готовых к I/O операциям ("ready list")
    std::vector<epoll_event> events(MAX_EVENTS);  // вектор сокетов/файлов для отслеживания

    set_nonblocking(file_fd);
    set_nonblocking(sock_fd);

    if (file_is_regular(file_fd)) {
        cout << "Reading input file before entering epoll..." << endl;
        n_bytes = Read(file_fd, buf, MAX_BUF_SIZE);
        eof_reached = true;
        cout << n_bytes << " bytes was read from file (stdin redirection was used)\n";
        Write_n_bytes_to_sock_fd(sock_fd, buf, n_bytes);
    } else {
        // Добавляем file_fd в epoll (для чтения)
        epoll_event file_ev{};
        file_ev.events = EPOLLIN;     // ждем события на ЧТЕНИЕ
        file_ev.data.fd = file_fd;    // привязываем событие к файловому дескриптору открытого файла
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, file_fd, &file_ev);  // добавляем в контекст опроса событий файловый дескриптор "file_fd"
    }

    // Добавляем sock_fd в epoll (для чтения)
    epoll_event sock_ev{};
    sock_ev.events = EPOLLIN;     // ждем события на ЧТЕНИЕ
    sock_ev.data.fd = sock_fd;    // привязываем событие к сокету
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &sock_ev);  // добавляем в контекст опроса событий сокет "sock_fd"

    for (;;) {
        int n_ready = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);

        for (int i = 0; i < n_ready; ++i) {
            int fd = events[i].data.fd;

            if (fd == file_fd) {
                n_bytes = Read(file_fd, buf, MAX_BUF_SIZE);

                if (n_bytes == 0) {  // все данные считаны; читать из файла больше нечего
                    eof_reached = true;
                    Shutdown(sock_fd, SHUT_WR);  // send FIN: больше новых данных нет - закрываем свою часть соединения
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, file_fd, nullptr);  // больше не нужно отслеживать файловый дескриптор
                }

                cout << n_bytes << " bytes was read from stdin\n";
                Write_n_bytes_to_sock_fd(sock_fd, buf, n_bytes);

            } else if (fd == sock_fd) {
                n_bytes = Recv(sock_fd, buf, MAX_BUF_SIZE, 0);

                if (n_bytes == 0) {
                    if (eof_reached) {
                        return;  // нормальное завершение: файл полностью прочитан и данных в сокете нет
                    } else {
                        cerr << "client_str_echo_using_epoll: server terminated prematurely" << endl;
                        exit(EXIT_FAILURE);
                    }
                }

                if (eof_reached) {
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
                    Shutdown(fd, SHUT_WR);
                    return;
                }
            }
        }  // end of INNER for loop
    } // end of outer for loop
}

void set_nonblocking(int sock_fd) {
    int flags = fcntl(sock_fd, F_GETFL, 0);

    if (flags == -1) {
        cerr << "fcntl(F_GETFL): " << ::strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    if (fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        cerr << "fcntl(F_SETFL, O_NONBLOCK): " << ::strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

bool file_is_regular(int file_fd) {
    struct stat st{};
    return fstat(file_fd, &st) == 0 && S_ISREG(st.st_mode);
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
        res = my_read(sock_fd, &ch);

        if (res == 1) {
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
    int n = select(n_fds, read_fds, write_fds, except_fds, timeout);
    if (n < 0) {
        cerr << "select error" << endl;
    }

    return (n);  // Возвращаемое значение м.б. равно нулю, если тайм-аут истек до того, как какие-либо файловые дескрипторы стали готовы.
}


void udp_srv_echo(int sock_fd, struct sockaddr_storage* udp_src_addr, socklen_t* udp_src_addr_len) {
    ssize_t n_bytes;
    char buf[MAX_BUF_SIZE];

    for (;;) {
        n_bytes = Recvfrom(sock_fd, buf, MAX_BUF_SIZE, 0,
                           (sockaddr*) udp_src_addr, udp_src_addr_len);

        auto [client_ip, client_port] = get_ip_port_from_addr_struct(*udp_src_addr);
        cout << "UDP-segment from client: [IP = " << client_ip << "], " << "[PORT = " << client_port << "]" << endl;

        Sendto(sock_fd, buf, n_bytes, 0,
               (sockaddr*) udp_src_addr, *udp_src_addr_len);
    }
}
