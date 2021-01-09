// ex16
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 08/10/20
// Copyright[2019]<Haokun Cai>

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include "./listen.h"

using namespace std;

void Usage(char *progname);

int main(int argc, char **argv) {
  if (argc != 2) {
    Usage(argv[0]);
  }

  // Create socket
  int sock_family;
  int listen_fd = Listen(argv[1], &sock_family);
  if (listen_fd == -1) {
    cerr << "listen or bind failed" << endl;
    return EXIT_FAILURE;
  }

  // If successful, receive client data in a infinte loop
  while (1) {
    struct sockaddr_storage caddr;
    socklen_t caddr_len = sizeof(caddr);
    int client_fd = accept(listen_fd,
                           reinterpret_cast<struct sockaddr *>(&caddr),
                           &caddr_len);
    if (client_fd < 0) {
      if ((errno == EAGAIN) || (errno == EINTR)) {
          continue;
      }
      cerr << "Failure on accept: " << strerror(errno) << endl;
      close(listen_fd);
      return EXIT_FAILURE;
    }

    HandleClient(client_fd,
                         reinterpret_cast<struct sockaddr *>(&caddr),
                         caddr_len,
                         sock_family);
    break;
  }

  close(listen_fd);
  return EXIT_SUCCESS;
}

void Usage(char *progname) {
  cerr << "usage: " << progname << " port" << endl;
  exit(EXIT_FAILURE);
}
