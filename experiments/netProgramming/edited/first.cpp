#include <iostream>

// for bzero
#include <strings.h>

// for socket  programming
#include <sys/socket.h>
#include <netinet/in.h>

// for getopt()
#include <unistd.h>

// for std::exit()
#include <cstdlib>

// for int inet_pton(int af, const char *src, void *dst);
#include <arpa/inet.h>

// my header
#include "my_unp.h"

using std::cout;
using std::endl;

<<<<<<< Updated upstream
int main(int argc, char** argv) {
  int sockfd{0};
  struct sockaddr_in	servaddr;
  int opt;
  char* ipaddr;
  char* port;

=======
int main(int argc, char* argv[]) {
  int sockfd;
  int n;
>>>>>>> Stashed changes
  char recvline[MAXLINE + 1];

  const char* options = "a:p:"; // available options, each of them takes an argument (indicated by semicolon)
  // char options[] = "a:p:"; // available options, each of them takes an argument (indicated by semicolon)

  // argc == argument counter, contains the number of arguments passed to the program.
  if(argc != 5) {
    cout << "Usage: <prog_name> -a <IP_address> -p <Port>" << endl;
    return -1;
  }

  while( (opt = getopt(argc, argv, options) ) != -1) { // вызываем getopt пока она не вернет -1
    switch(opt) {
    case 'a':
      ipaddr = optarg;
      cout << "IP address = " << optarg << endl;
      break;
    case 'p':
      port = optarg;
      cout << "Port = " << optarg << endl;
      break;
    }
  } // end getopt()

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    cout << "Socket error." << endl;
    std::exit(EXIT_FAILURE);
  }
  else {
    cout << "Socket was created successfully. " << endl;
  }

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;

  // strtol - to convert char* (C-string) to uint16_t
  servaddr.sin_port = htons(strtol(port, NULL, 10));
  if (inet_pton(AF_INET, ipaddr, &servaddr.sin_addr) <= 0) {
    cout << "inet_pton errro for " << ipaddr << endl;;
    std::exit(EXIT_FAILURE);
  }

  if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
      cout << "bind failed." << endl;
      std::exit(EXIT_FAILURE);
  }

  if (listen(sockfd, 3) < 0)
    {
      perror("listen");
      exit(EXIT_FAILURE);
    }


  cout << "Now the value of servaddr.sin_addr = " << std::hex << servaddr.sin_addr.s_addr << endl;
  return 0;
}
