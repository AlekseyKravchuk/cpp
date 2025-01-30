//
// Created by kav on 16.01.25.
//
#include <iostream>
#include <cstring>       // memset
#include <sys/socket.h>  // socket
#include <arpa/inet.h>
#include <unistd.h>      // close
#include <fstream>
#include <filesystem>
#include <unistd.h>      // getpid()
#include <net/if.h>      // setsockopt

#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;
namespace fs = std::filesystem;

int Socket(int domain, int type, int protocol) {
    int socket_fd;

    if ((socket_fd = socket(domain, type, protocol)) < 0) {
        cerr << "socket function error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

int Inet_pton(int af, const char *src, void *dst) {
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

const char* Inet_ntop(int af, const void *src,
                      char *dst, socklen_t size) {
    const char* ptr = inet_ntop(af, src, dst, size);
    if (ptr == nullptr) {
        cerr << "Client address could not be converted to string representation (inet_ntop): "
             << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return dst;
}

int Connect(int socket_fd,
            const struct sockaddr *server_address_casted,
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

ssize_t Recv(int socket_fd, void *buffer, size_t buffer_size, int flags) {

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
    if ( ptr != nullptr) {
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

    if ( client_socket_fd < 0) {
        cerr << "accept connection error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return client_socket_fd;
}

ssize_t Send(int socket_fd, const void *buf, size_t len, int flags=0) {
    ssize_t num_bytes_sent = 0;

    // !!! Успешное завершение "send()" ещё не означает, что процесс на другом конце соединения получил отправленные данные!!!
    // Всё, что гарантирует "send()" - это отсутствие ошибок при передаче данных сетевым драйверам.
    if ( (num_bytes_sent = send(socket_fd, buf, len, flags)) < 0) {
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

void client_check_arguments(int argc,
                            char* argv[],
                            string& server_ip,
                            uint16_t& server_port) {
    // Declare the supported options using the options_description class.
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("ipaddr,i", po::value<string>()->required(), "server IP address to connect to")
            ("port,p",   po::value<uint16_t>()->required(), "server port to connect to");

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

        std::cout << "Client (PID = " << getpid() << ") is trying to connect to server at "
                  << server_ip << ":" << server_port << endl;

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

void server_check_arguments(int argc,
                            char* argv[],
                            uint16_t& port_listen_to,
                            std::string& file_path) {
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("port,p", po::value<uint16_t>()->required(), "port to listen on")
            ("file,f", po::value<std::string>()->required(), "path to file containing text to be sent to clients");

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
        file_path = vars_map["file"].as<std::string>();

        std::cout << "Command args successfully parsed, port to listen to (server side): " << port_listen_to << "\n";
        std::cout << "File to send: " << file_path << "\n";

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

string get_content(const std::string& file_path) {
    std::ifstream file_in(file_path);
    if (!file_in.is_open()) {
        ostringstream oss;
        oss << "Failed to open file: \"" << file_path
            << "\" (Reason: " << strerror(errno) << ")." << endl;
        throw std::runtime_error(oss.str());
    }

    std::string file_content( (std::istreambuf_iterator<char>(file_in)),
                              std::istreambuf_iterator<char>() );
    file_in.close();

    return file_content;
}

void restrict_to_iface(int server_fd,
                       int level,
                       int optname,
                       const void* optval,
                       const socklen_t optlen) {
    if (setsockopt(server_fd, level, optname, optval, optlen) < 0) {
        // (setsockopt(server_fd, SOL_SOCKET, SO_BINDTODEVICE, "eth0", strlen("eth0")) < 0)
        perror("Bind to device failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
}
