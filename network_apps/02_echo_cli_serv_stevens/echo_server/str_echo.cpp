#include    "unp.h"

//void str_echo(int sockfd) {
//    ssize_t n;
//    char buf[MAXLINE];
//
//    again:
//    while ((n = read(sockfd, buf, MAXLINE)) > 0)
//        Writen(sockfd, buf, n);
//
//    if (n < 0 && errno == EINTR)
//        goto again;
//    else if (n < 0)
//        err_sys("str_echo: read error");
//}


void str_echo(int sockfd) {
    ssize_t n;
    char buf[MAXLINE];

    while (true) {
        n = read(sockfd, buf, MAXLINE);
        if (n > 0) {
            Writen(sockfd, buf, n);
        } else if (n < 0 && errno == EINTR) {
            continue; // Повторяем попытку чтения
        } else if (n < 0) {
            err_sys("str_echo: read error");
            break;
        } else {
            break; // n == 0, клиент закрыл соединение
        }
    }
}


