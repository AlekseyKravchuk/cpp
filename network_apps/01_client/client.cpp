#include <ctime>
#include <string>
#include <cstdarg>   // va_start
#include <syslog.h>	 // syslog()
#include <iostream>


#include "wrappers.h"

using namespace std;

int main(int argc, char* argv[]) {
    const size_t MAXLINE = 4096; // max text line length
    int sockfd;
    int n;
    int counter = 0;
    char recvline[MAXLINE + 1];
    struct sockaddr_in	cli_addr{};

    if (argc != 2) {
        cerr << "usage: a.out <IPaddress>" << endl;
        exit(EXIT_FAILURE);
    }

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "socket error" << endl;
        exit(EXIT_FAILURE);
    }

    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port   = htons(13);	/* daytime server */
    if (inet_pton(AF_INET, argv[1], &cli_addr.sin_addr) <= 0) {
        cerr << "inet_pton error for " << argv[1] << endl;
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr* ) &cli_addr, sizeof(cli_addr)) < 0) {
        cerr << "connect error" << endl;
        exit(EXIT_FAILURE);
    }

    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        counter++;
        recvline[n] = 0;	/* null terminate */

        if (fputs(recvline, stdout) == EOF) {
            cerr << "fputs error" << endl;
            exit(EXIT_FAILURE);
        }

    }

    if (n < 0) {
        cerr << "read error" << endl;
        exit(EXIT_FAILURE);
    }


    cout << counter << endl;

    return 0;
}