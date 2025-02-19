#include <iostream>
#include <cstring>       // memset
#include <sys/socket.h>  // socket
#include <arpa/inet.h>
#include <unistd.h>      // close


#include "utilities.h"

using namespace std;

int Socket(int domain, int type, int protocol) {
    int socket_fd;

    if ((socket_fd = socket(domain, type, protocol)) < 0) {
        cerr << "socket function error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

int Inet_pton(int af, const char* src, void* dst) {
    int result = inet_pton(af, src, dst);

    if (result == 0) {
        cerr << "Invalid server IP-address: " << src << endl;
        exit(EXIT_FAILURE);
    } else if (result == -1) {
        cerr << "Address family is not supported: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return result;
}

const char* Inet_ntop(int af, const void* src,
                      char* dst, socklen_t size) {
    const char* ptr = inet_ntop(af, src, dst, size);
    if (ptr == nullptr) {
        cerr << "Client address could not be converted to string representation (inet_ntop): "
             << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return dst;
}

int Connect(int socket_fd,
            const struct sockaddr* server_address_casted,
            socklen_t addrlen) {

    // If the connection or binding succeeds, zero is returned.  On error, -1 is returned, and errno is set appropriately.
    int result = connect(socket_fd, server_address_casted, addrlen);

    if (result < 0) {
        cerr << "There was an error making a connection to the remote socket (\"connect\" function): "
             << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return result;
}

ssize_t Recv(int socket_fd, void* buffer, size_t buffer_size, int flags) {

    // Если recv(read) возвращает 0, это значит, что сервер закрыл соединение.
    ssize_t bytes_read = recv(socket_fd, buffer, buffer_size, 0);

    if (bytes_read == 0) {
        cout << "Connection closed by server." << endl;
    } else if (bytes_read < 0) {
        cerr << "socket read error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return bytes_read;
}

void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    int result = bind(sockfd, addr, addrlen);

    if (result == -1) {
        cerr << "bind error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Listen(int sock_fd, int backlog) {
    char* ptr = getenv("LISTENQ");
    char** end_ptr = nullptr;

    // May be override 2nd argument with environment variable
    if (ptr != nullptr) {
        backlog = static_cast<int>(strtol(ptr, end_ptr, 10));
    }

    int result = listen(sock_fd, backlog);

    if (result == -1) {
        cerr << "listen failed: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

int Accept(int listen_fd, struct sockaddr* sa, socklen_t* salenptr) {
    int client_socket_fd = accept(listen_fd, sa, salenptr);

    if (client_socket_fd < 0) {
        cerr << "accept connection error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return client_socket_fd;
}

ssize_t Send(int socket_fd, const void* buf, size_t len, int flags = 0) {
    ssize_t num_bytes_sent = 0;

    // !!! Успешное завершение "send()" ещё не означает, что процесс на другом конце соединения получил отправленные данные!!!
    // Всё, что гарантирует "send()" - это отсутствие ошибок при передаче данных сетевым драйверам.
    if ((num_bytes_sent = send(socket_fd, buf, len, flags)) < 0) {
        cerr << "Sending data error (send call): " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return num_bytes_sent;
}

void Close(int fd) {
    if (close(fd) == -1) {
        cerr << "close error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Write_n_bytes_to_sock_fd(int sock_fd, void* buf_start, size_t n_bytes) {
    if (write_n_bytes_to_sock_fd(sock_fd, buf_start, n_bytes) != n_bytes) {
        cerr << "write_n_bytes_to_sock_fd error" << endl;
        exit(EXIT_FAILURE);
    }
}

/*
 * char *fgets(char* str, int size, FILE* file_stream);
 * стандартная функция из <stdio.h> в POSIX-системах, предназначенная для безопасного чтения строк из потока.
 * char *str (send_buf)     — буфер, в который записывается считанная строка;
 * int size  (MAX_BUF_SIZE) — максимальное количество символов, которые можно записать (включая завершающий \0);
 * FILE *file_stream (fp)   — указатель на поток (stdin, fd из fopen() и т.д), ИЗ КОТОРОГО считываются данные.
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
char* Fgets(char* str_buf, int n, FILE* file_stream) {
    char* ptr = fgets(str_buf, n, file_stream);

    if (ptr == nullptr && ferror(file_stream)) {
        cerr << "fgets error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void Fputs(const char* str, FILE* stream) {
    if (!str || !stream) {
        cerr << "Fputs error: nullptr." << endl;
        exit(EXIT_FAILURE);
    }

    // Защищаем доступ к файловому потоку
    flockfile(stream);   // Блокируем поток
    // fputs() writes the string "str" to file stream, without its terminating null byte ('\0').
    int result = fputs(str, stream);
    funlockfile(stream); // Разблокируем поток

    if (result == EOF) {
        cerr << "fputs error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

ssize_t Write(int fd, const void* recv_buf, size_t recv_buf_len) {
    ssize_t bytes_count = write(fileno(stdout), recv_buf, recv_buf_len);
    if (bytes_count == -1) {
        cerr << "write error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return bytes_count;
}

ssize_t Readline(int fd, void* ptr_to_buf, size_t max_len) {
    ssize_t n;

    if ((n = readline(fd, ptr_to_buf, max_len)) < 0) {
        cerr << "readline error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return n;
}

pid_t Fork() {
    pid_t	pid;

    if ( (pid = fork()) == -1) {
        cerr << "fork error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return(pid);
}


