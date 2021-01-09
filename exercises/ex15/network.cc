// ex15
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 08/10/20
// Copyright[2019]<Haokun Cai>


#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include "./network.h"
using namespace std;

// adapt from lecture example
bool LookupName(char *name,
                unsigned short port,
                struct sockaddr_storage *ret_addr,
                size_t *ret_addrlen) {
  struct addrinfo hints, *results;
  int retval;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  // Do the lookup by invoking getaddrinfo().
  if ((retval = getaddrinfo(name, nullptr, &hints, &results)) != 0) {
    cerr << "getaddrinfo failed: " << gai_strerror(retval) << endl;
    return false;
  }

  // Set the port in the first result.
  if (results->ai_family == AF_INET) {
    struct sockaddr_in *v4addr =
            reinterpret_cast<struct sockaddr_in *>(results->ai_addr);
    v4addr->sin_port = htons(port);
  } else if (results->ai_family == AF_INET6) {
    struct sockaddr_in6 *v6addr =
            reinterpret_cast<struct sockaddr_in6 *>(results->ai_addr);
    v6addr->sin6_port = htons(port);
  } else {
    cerr << "getaddrinfo failed to provide an IPv4 or IPv6 address";
    cerr << endl;
    freeaddrinfo(results);
    return false;
  }

  // Return the first result.
  assert(results != nullptr);
  memcpy(ret_addr, results->ai_addr, results->ai_addrlen);
  *ret_addrlen = results->ai_addrlen;

  // Clean up.
  freeaddrinfo(results);
  return true;
}

// adapt from lecture example connect
bool Connect(const struct sockaddr_storage &addr,
             const size_t &addrlen,
             int *ret_fd) {
  // Create the socket.
  int socket_fd = socket(addr.ss_family, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    cerr << "socket() failed: " << strerror(errno) << endl;
    return false;
  }

  // Connect the socket to the remote host.
  int res = connect(socket_fd,
                    reinterpret_cast<const sockaddr *>(&addr),
                    addrlen);
  if (res == -1) {
    cerr << "connect() failed: " << strerror(errno) << endl;
    return false;
  }

  *ret_fd = socket_fd;
  return true;
}

// a wrapper for read to deal with error
int ReadWrapper(int fd, char* buf, int readlen) {
    int res;
    while (1) {
    res = read(fd, buf, readlen);
    if (res == -1) {
      if ((errno == EAGAIN) || (errno == EINTR)) {
        continue;
      } else {
        cerr << "read() failed: " << strerror(errno) << endl;
        break;
      }
    }
  }
  return res;
}

// a warpper for write to deal with write
int WriteWrapper(int fd, char* buf, int writelen) {
    int res;
    int written = 0;
    while (written < writelen) {
    res = write(fd, buf + written, writelen - written);
    if (res == -1) {
      if ((errno == EAGAIN) || (errno == EINTR)) {
        continue;
      } else {
      cerr << "write() failed: " << strerror(errno) << endl;
      break;
      }
    }
    if (res == 0) {
        cerr << "write() failed"  << endl;
        break;
    }

    written += res;
  }
  return written;
}
