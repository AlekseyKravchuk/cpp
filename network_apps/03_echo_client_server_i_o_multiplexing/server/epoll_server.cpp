#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "wrappers.h"
#include "utilities.h"
#include "parsing.h"

using namespace std;

#define MAX_BUF_SIZE 4'096
#define MAX_EVENTS   1'000

int main(int argc, char** argv) {
    uint16_t port_listen_to = 0;
    constexpr int LISTEN_QUEUE_LEN = 100;
    int listen_fd;
    char buf[MAX_BUF_SIZE];

    echo_server_check_arguments(argc, argv, port_listen_to);
    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);
    make_socket_nonblocking(listen_fd);

    struct sockaddr_in server_address = {
            .sin_family = AF_INET,
            .sin_port = htons(port_listen_to),
            .sin_addr = {INADDR_ANY},
            .sin_zero = {}
    };

    Bind(listen_fd, (struct sockaddr*)&server_address, sizeof(server_address));
    Listen(listen_fd, LISTEN_QUEUE_LEN);

    // Инициализируем контекст опроса событий - создаем экземпляр epoll, который будет отслеживать файловые дескрипторы.
    // Когда файловый дескриптор создается с флагом O_CLOEXEC, он автоматически закрывается при выполнении execve(),
    // если текущий процесс порождает новый исполняемый процесс - это предотвращает случайную передачу файловых дескрипторов потомкам.
    int epoll_fd = Epoll_create1(EPOLL_CLOEXEC);

    // Буфер, на который указывает "events.data()" используется для возвращения (функцией "epoll_wait")
    // информации из "ready list" - из списка дескрипторов, готовых к I/O операциям
    std::vector<epoll_event> events(MAX_EVENTS);  // вектор сокетов/файлов для отслеживания

    // Объект "ev" используется для добавления/удаления/модификации файловых дескрипторов в epoll
    // и задания типа событий, которые нас интересуют (в данном случае - событие на чтение, EPOLLIN).
    epoll_event ev{};
    // ev.events = EPOLLIN;         // ждем события на ЧТЕНИЕ

    // При использовании epoll с edge-triggered (EPOLLET) режимом:
    // - epoll_wait() сигнализирует о событии только один раз, когда новое соединение доступно;
    // - если accept() в этот момент не обработает все входящие соединения, следующие вызовы epoll_wait()
    //   не будут уведомлять о новых подключениях;
    ev.events = EPOLLIN | EPOLLET;  // ждем события на ЧТЕНИЕ + используем Edge-triggered mode (EPOLLET)

    ev.data.fd = listen_fd;  // привязываем событие к прослушиваемому (listening, серверному) сокету

    // "epoll_ctl" добавляет (EPOLL_CTL_ADD) в контекст опроса событий сокет "listen_fd"
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

    // TODO: добавить обработку клиентских сокетов
    for (;;) {
        // Обработка событий с помощью epoll_wait(), который блокирует выполнение, ожидая события от подключенных клиентов.
        int n_ready = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);

        for (int i = 0; i < n_ready; i++) {
            int fd = events[i].data.fd;

            // Поскольку используем edge-triggered (EPOLLET) режим, то внутри epoll-цикла после срабатывания события
            // на listen_sock нужно обработать принять все ожидающие подключения в цикле while
            if (fd == listen_fd) {
//                for(;;) {
//                    sockaddr_storage client_addr{};
//                    socklen_t client_addr_len = sizeof(client_addr);
//
//                    int connected_fd = Accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
//                    make_socket_nonblocking(connected_fd);
//                    print_info_about_connected_client(client_addr);
//
//                    epoll_event client_event{};
//                    client_event.events = EPOLLIN;
//                    client_event.data.fd = connected_fd;
//                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connected_fd, &client_event);
//                }

            // А что если без цикла:
                sockaddr_storage client_addr{};
                socklen_t client_addr_len = sizeof(client_addr);

                int connected_fd = Accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
                make_socket_nonblocking(connected_fd);
                print_info_about_connected_client(client_addr);

                epoll_event client_event{};
                client_event.events = EPOLLIN;
                client_event.data.fd = connected_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connected_fd, &client_event);
            } else {
                ssize_t n_bytes = Read(fd, buf, MAX_BUF_SIZE);
                if (n_bytes == 0) {
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
                } else {
                    Write_n_bytes_to_sock_fd(fd, buf, static_cast<size_t>(n_bytes));
                    shutdown(fd, SHUT_WR);
                }
            }
        }
    }
}

