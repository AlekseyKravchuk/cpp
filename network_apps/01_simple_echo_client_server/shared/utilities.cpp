#include <iostream>
#include <tuple>
#include <sys/types.h>
#include <unistd.h>

#include "utilities.h"
#include "wrappers.h"

#define MAX_BUF_SIZE 4096  /* Максимальный размер строки (буфера, в который осуществляется чтение из сокета) */

using namespace std;

// ############################## Server-related ##############################
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
    ssize_t n_bytes_read;
    char buf[MAX_BUF_SIZE];
    bool should_retry = false;

    while (true) {
        // Считываем данные из сокета "sock_fd" в буфер "buf"
        n_bytes_read = recv(sock_fd, buf, MAX_BUF_SIZE, 0);

        if (n_bytes_read > 0) {
            cout << "Успешно прочитано из сокета " << n_bytes_read << " байт.\n"
                 << "Содержимое: " << string(buf, n_bytes_read-1) << endl;
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

// ############################## Client-related ##############################
void client_str_echo(FILE* stdin_fp, int sock_fd) {
    char send_buf[MAX_BUF_SIZE];
    char recv_buf[MAX_BUF_SIZE];

    while ((Fgets(send_buf, MAX_BUF_SIZE, stdin_fp)) != nullptr) {
        // "Write_n_bytes_to_sock_fd" отправляет серверу строку, которую считала "Fgets" и записала в буфер "send_buf".
        Write_n_bytes_to_sock_fd(sock_fd, send_buf, strlen(send_buf));

        // TODO: fix buggy Readline function (readline reads the line echoed back from the server)
        if (Readline(sock_fd, recv_buf, MAX_BUF_SIZE) == 0) {
            cerr << "client_str_echo: server terminated prematurely: " << endl;
            exit(EXIT_FAILURE);
        }

        Fputs(recv_buf, stdout);  // TODO: заменить fputs на потокобезопасную функцию
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
                break;    /* newline is stored, like fgets() */
        } else if (res == 0) {
            *pos_ptr = 0;
            return (w_count - 1);    /* EOF, w_count - 1 bytes were read */
        } else
            return (-1);        /* error, errno set by read() */
    }

    *pos_ptr = 0;    /* null terminate like fgets() */

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