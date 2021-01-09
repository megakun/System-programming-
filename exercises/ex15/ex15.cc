
// ex15
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 08/10/20
// Copyright[2019]<Haokun Cai>

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

#include "./network.h"
#define BUF_SIZE 512

using namespace std;

void Usage(char *progname);

int main(int argc, char** argv) {
    if (argc != 4) {
        Usage(argv[0]);
    }

    unsigned short port = 0;
    if (sscanf(argv[2], "%hu", &port) != 1) {
        Usage(argv[0]);
    }

    // Get an appropriate sockaddr structure
    struct sockaddr_storage addr;
    size_t addrlen;
    if (!LookupName(argv[1], port, &addr, &addrlen)) {
        cerr << "LookupName() " << argv[1] << " failed." << endl;
        Usage(argv[0]);
    }

    // Connect socket to remote host
    int socket_fd;
    if (!Connect(addr, addrlen, &socket_fd)) {
        cerr << "Connect() " << argv[1] << " failed." << endl;
        Usage(argv[0]);
    }

    // Read the local file
    int file_fd = open(argv[3], O_RDONLY);
    if (file_fd == -1) {
        cerr << "open() " << argv[3] << " failed." << endl;
        Usage(argv[0]);
    }

    // Read from remote host
    char buf[BUF_SIZE];
    while (1) {
        int resRead = ReadWrapper(file_fd, buf, BUF_SIZE);
        int resWrite = WriteWrapper(socket_fd, buf, resRead);
        if (resWrite != resRead) {
            cout << "write() failed" << endl;
            break;
        }
    }
    // Clean up.
    close(socket_fd);
    close(file_fd);
    return EXIT_SUCCESS;
}

void Usage(char *progname) {
    cerr << "usage: " << progname << " hostname port" << endl;
    exit(EXIT_FAILURE);
}
