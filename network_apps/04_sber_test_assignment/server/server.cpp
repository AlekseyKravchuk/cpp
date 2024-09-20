#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <cstring>
#include <arpa/inet.h>  // Для работы с сокетами

constexpr int SERVER_PORT = 12345;
constexpr int BUFFER_SIZE = 1024;

// Класс сервера
class UDP_Server {
  public:
    explicit UDP_Server(uint16_t port) : _port(port) {}

    // Запуск сервера
    void Start() {
        LoadResources("resources.txt");
        CreateSocket();
        BindSocket();
        Run();
    }

  private:
    int _socket_fd;
    uint16_t _port;
    std::unordered_map<std::string, std::string> _resources;

    // Загрузка ресурсов из файла
    void LoadResources(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл ресурсов." << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string resource_name, resource_content;
            if (iss >> resource_name >> std::ws) {
                std::getline(iss, resource_content);
                _resources[resource_name] = resource_content;
            }
        }
    }

    // Создание сокета
    void CreateSocket() {
        _socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_socket_fd < 0) {
            throw std::runtime_error("Не удалось создать сокет.");
        }
    }

    // Привязка сокета к порту
    void BindSocket() const {
        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(_port);

        if (bind(_socket_fd, (sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
            close(_socket_fd);
            throw std::runtime_error("Не удалось привязать сокет.");
        }

        std::cout << "Сервер запущен на порту " << _port << std::endl;
    }

    // Основной цикл работы сервера
    void Run() {
        while (true) {
            sockaddr_in client_addr{};
            socklen_t client_len = sizeof(client_addr);
            char buffer[BUFFER_SIZE];

            ssize_t recv_len = recvfrom(_socket_fd, buffer, BUFFER_SIZE, 0, (sockaddr*) &client_addr, &client_len);
            if (recv_len > 0) {
                std::thread(&UDP_Server::HandleClientRequest, this, client_addr, buffer, recv_len).detach();
            }
        }
    }

    // Обработка запросов клиентов
    void HandleClientRequest(sockaddr_in client_addr, char* buffer, ssize_t recv_len) {
        buffer[recv_len] = '\0';  // Завершаем строку

        std::string resource_name(buffer);
        std::string response;

        auto it = _resources.find(resource_name);
        if (it != _resources.end()) {
            response = "-BEGIN-\n" + it->second + "\n-END-\n";
        } else {
            response = "-ERROR-\nResource not found\n-END-\n";
        }

        sendto(_socket_fd, response.c_str(), response.size(), 0, (sockaddr*) &client_addr, sizeof(client_addr));
    }
};

int main() {
    try {
        UDP_Server server(SERVER_PORT);
        server.Start();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
