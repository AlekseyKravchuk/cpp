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

    // Установка опции "SO_REUSEADDR" позволяет серверу быстро перезапускаться без ожидания завершения старых соединений.
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_address = {
            .sin_family = AF_INET,
            .sin_port = htons(port_listen_to),
            .sin_addr = {INADDR_ANY},
            .sin_zero = {}
    };

    Bind(listen_fd, (struct sockaddr*) &server_address, sizeof(server_address));
    Listen(listen_fd, LISTEN_QUEUE_LEN);
    set_nonblocking(listen_fd);  // make listening socket to be nonblocking

    // Инициализируем контекст опроса событий - создаем экземпляр epoll, который будет отслеживать файловые дескрипторы.
    int epoll_fd = Epoll_create1(EPOLL_CLOEXEC);

    // Буфер "events.data()" используется для возвращения из "epoll_wait" информации из списка дескрипторов, готовых к I/O операциям ("ready list")
    std::vector<epoll_event> events(MAX_EVENTS);  // вектор сокетов/файлов для отслеживания

    // Объект "ev" используется для добавления/удаления/модификации файловых дескрипторов в epoll
    // и задания типа событий, которые нас интересуют (в данном случае - событие на чтение, EPOLLIN).
    epoll_event ev{};
    ev.events = EPOLLIN | EPOLLET; // ждем события на ЧТЕНИЕ + используем Edge-triggered mode (EPOLLET)
    ev.data.fd = listen_fd;        // привязываем событие к прослушиваемому (listening, серверному) сокету
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);  // добавляем в контекст опроса событий сокет "listen_fd"

    for (;;) {
        // Обработка событий с помощью epoll_wait(), который блокирует выполнение, ожидая события от подключенных клиентов.
        // В режиме edge-triggered (EPOLLET) epoll_wait() сообщает о событии только один раз, когда появляются новые данные.
        // После этого ядро не будет повторно уведомлять, пока не появится новая активность (например, новый клиент в очереди accept).
        // Если во время обработки события EPOLLIN не обработать все входящие подключения, то сервер просто не узнает,
        // что в очереди остались клиенты.
        // В результате при следующем вызове accept(), когда очередь уже пуста, мы получим EAGAIN / EWOULDBLOCK.
        int n_ready = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);

        for (int i = 0; i < n_ready; i++) {
            // Проверяем, произошла ли ошибка (EPOLLERR) или был ли закрыт удалённый конец соединения (EPOLLHUP):
            // EPOLLERR: ошибка на сокете. Например, если клиент неожиданно отключился или произошёл сбой сети.
            // EPOLLHUP: клиент закрыл соединение (close() с его стороны), но сервер ещё не вызвал read(), чтобы получить EOF.
            if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP) {
                close(events[i].data.fd);
                continue;
            }

            int fd = events[i].data.fd;
            if (fd == listen_fd) {  // обработка  очереди входящих соединений в режиме edge-triggered (EPOLLET)
                for (;;) {  // пока есть клиенты в очереди входящих соединений, крутимся в цикле
                    // В режиме EPOLLET после срабатывания события на listen_fd внутри epoll-цикла нужно принять все
                    // ожидающие подключения в цикле for(;;), т.к. в режиме edge-triggered ядро уведомит нас только
                    // один раз, когда появляются новые данные.
                    // Если мы не вычитаем все данные из буфера, следующего уведомления мы не получим!
                    sockaddr_storage client_addr{};
                    socklen_t client_addr_len = sizeof(client_addr);

                    int connected_fd = accept(listen_fd, (struct sockaddr*) &client_addr, &client_addr_len);

                    if (connected_fd == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            cerr << "No pending connections, breaking the loop for processing incoming connections"
                                 << endl;
                            break;  // Очередь пуста, выходим из цикла обработки входящих соединений
                        } else {
                            cerr << R"(Other than the "EAGAIN" accept connection error: )" << strerror(errno) << endl;
                            continue;
                        }
                    }

                    set_nonblocking(connected_fd);  // make connected socket to be nonblocking
                    print_info_about_connected_client(client_addr);

                    epoll_event client_event{};
                    client_event.events = EPOLLIN | EPOLLET;
                    client_event.data.fd = connected_fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connected_fd, &client_event);
                }
            } else if (fd != listen_fd) {  // вычитываем все данные из клиентского сокета
                while (true) {
                    // Цикл нужен, т.к. EPOLLET уведомляет нас только один раз, когда появляются новые данные.
                    // Если мы не вычитаем все данные из буфера recv(), следующего уведомления мы не получим!
                    // !!! Нужно читать до тех пор, пока не получим EAGAIN !!!:
                    ssize_t n_bytes = recv(fd, buf, MAX_BUF_SIZE, 0);

                    if (n_bytes == -1) {
                        // Этот if:
                        //    - Защищает от ситуации, когда recv() вызывается на пустом буфере.
                        //    - Гарантирует, что цикл не зависнет в recv(), если данные временно недоступны.
                        //    - Позволяет корректно закрывать соединение при критических ошибках.
                        // => EAGAIN (Resource temporarily unavailable) — означает, что операция recv() выполнена
                        //    в неблокирующем режиме и на данный момент нет доступных данных.
                        // =>  EWOULDBLOCK — в современных Linux EWOULDBLOCK и EAGAIN эквивалентны.
                        // Они не являются фатальными ошибками! Просто буфер пуст, и мы должны прекратить чтение.
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;  // Данные в сокете закончились, мы вычитали все данные, можно выходить из цикла
                        } else {    // получили критическую ошибку
                            cerr << R"(Other than the "EAGAIN/EWOULDBLOC" recv error: )" << strerror(errno) << endl;
                            close(fd);
                            break;
                        }
                    } else if (n_bytes == 0) {  // удаленная сторона (клиент) закрыла соединение
                        // В Berkeley Sockets (TCP-сокеты) закрытие соединения происходит поэтапно.
                        // Когда клиент вызывает close() или shutdown(fd, SHUT_WR), сервер не сразу получает ошибку, а сначала получает EOF (конец файла).
                        // EOF в recv() означает, что:
                        //    - Клиент закрыл соединение => сервер больше не должен использовать этот сокет.
                        //    - В TCP больше не будет данных от клиента.
                        //    - Мы обязаны закрыть fd на сервере.
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) == -1) {
                            cerr << "epoll_ctl DEL" << endl;
                        }
                        close(fd);  // Нет смысла оставлять fd открытым после EOF.
                        break;      // всё, вычитали все данные из клиентского сокета => выходим из цикла
                    } else if (n_bytes > 0) {
                        Write_n_bytes_to_sock_fd(fd, buf, static_cast<size_t>(n_bytes));
                    }
                }
            }
        }
    }
}

