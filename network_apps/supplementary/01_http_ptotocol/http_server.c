#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    enum {
        BUF_SIZE = 1024,
        TWICE_BUF_SIZE = 2048
    };

    FILE *file;
    size_t bytes_read;
    char response_body[BUF_SIZE];
    char http_header[BUF_SIZE] = "HTTP/1.1 200 OK\r\n\n";  // response header
    char whole_response[TWICE_BUF_SIZE];

    // Открываем файл для чтения в бинарном режиме
    file = fopen("index.html", "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return 1;
    }

    // Читаем содержимое файла в буфер "response_body"
    bytes_read = fread(response_body, 1, BUF_SIZE - 1, file);
    if (ferror(file)) {
        perror("Ошибка при чтении файла");
        fclose(file);
        return 1;
    }

    fclose(file);  // Закрываем файл

    // Добавляем нулевой символ в конец строки (если читаем текстовый файл)
    response_body[bytes_read] = '\0';

    snprintf(whole_response, sizeof(whole_response), "%s%s", http_header, response_body);
    // ##################################################

    uint16_t port_listen_to = 3444;
    const uint32_t MAX_QUEUE_LENGTH = 100;
    int listening_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_listen_to);  // convert port_number from host byte order to network byte order.
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(listening_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    listen(listening_socket, MAX_QUEUE_LENGTH);

    int connected_socket;

    // используем универсальную структуру адреса для сохранения информации о подключившемся клиенте
    struct sockaddr_storage client_address = {0};
    socklen_t client_address_len = sizeof(client_address);

    for (;;) {
        connected_socket = accept(listening_socket, (struct sockaddr*) &client_address, &client_address_len);
        send(connected_socket, (void*) whole_response, sizeof(whole_response), 0);
        close(connected_socket);
    }

    return 0;
}
