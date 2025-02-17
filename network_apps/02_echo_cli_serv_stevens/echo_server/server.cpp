#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>

#include	"unp.h"

using namespace std;

int main() {
    int listen_fd, conn_fd;
    pid_t child_pid;
    socklen_t client_addr_len;
    struct sockaddr_in client_addr{};
    struct sockaddr_in server_addr{};

    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERV_PORT);

    Bind(listen_fd, (SA * ) & server_addr, sizeof(server_addr));

    Listen(listen_fd, LISTENQ);

    cout << "Server is listening at port: " << SERV_PORT << endl;

    for (;;) {
        client_addr_len = sizeof(client_addr);
        conn_fd = Accept(listen_fd, (SA * ) & client_addr, &client_addr_len);

        if ((child_pid = Fork()) == 0) {    /* child process */
            Close(listen_fd);    /* close listening socket */
            str_echo(conn_fd);    /* process the request */
            Close(listen_fd);
            exit(0);
        }

        Close(conn_fd);            /* parent closes connected socket */
    }
}
