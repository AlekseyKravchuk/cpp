#include <iostream>
#include <string>
#include <cstring>
#include <utility>
#include <arpa/inet.h>  // Для работы с сокетами
#include <unistd.h>

constexpr int SERVER_PORT = 12345;
constexpr int BUFFER_SIZE = 1024;

// Класс клиента
class UDP_Client {
  public:
    UDP_Client(std::string  server_ip, uint16_t port) :
            _server_ip(std::move(server_ip)), _port(port) {
        CreateSocket();
    }

    ~UDP_Client() {
        close(_socket_fd);
    }

    // Отправка запроса и получение ответа
    void RequestResource(const std::string& resource_name) {
        SendRequest(resource_name);
        ReceiveResponse();
    }

  private:
    int _socket_fd;
    std::string _server_ip;
    uint16_t _port;

    // Создание UDP сокета
    void CreateSocket() {
        _socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_socket_fd < 0) {
            throw std::runtime_error("Не удалось создать сокет.");
        }
    }

    // Отправка запроса серверу
    void SendRequest(const std::string& resource_name) {
        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(_port);
        server_addr.sin_addr.s_addr = inet_addr(_server_ip.c_str());

        if (sendto(_socket_fd, resource_name.c_str(), resource_name.size(), 0, (sockaddr*) &server_addr,
                   sizeof(server_addr)) < 0) {
            throw std::runtime_error("Не удалось отправить запрос серверу.");
        }
    }

    // Получение ответа от сервера
    void ReceiveResponse() const {
        char buffer[BUFFER_SIZE];
        sockaddr_in from_addr{};
        socklen_t from_len = sizeof(from_addr);
        ssize_t recv_len = recvfrom(_socket_fd, buffer, BUFFER_SIZE, 0, (sockaddr*) &from_addr, &from_len);

        if (recv_len > 0) {
            buffer[recv_len] = '\0';  // Завершаем строку
            std::cout << "Ответ сервера:\n" << buffer << std::endl;
        } else {
            throw std::runtime_error("Не удалось получить ответ от сервера.");
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Использование: " << argv[0] << " <resource_name>" << std::endl;
        return -1;
    }

    std::string resource_name = argv[1];

    try {
        UDP_Client client("127.0.0.1", SERVER_PORT);
        client.RequestResource(resource_name);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
