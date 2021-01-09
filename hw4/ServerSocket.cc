/*
 * Copyright ©2020 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Summer Quarter 2020 for purptroses of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>       // for snprintf()
#include <unistd.h>      // for close(), fcntl()
#include <sys/types.h>   // for socket(), getaddrinfo(), etc.
#include <sys/socket.h>  // for socket(), getaddrinfo(), etc.
#include <arpa/inet.h>   // for inet_ntop()
#include <netdb.h>       // for getaddrinfo()
#include <errno.h>       // for errno, used by strerror()
#include <string.h>      // for memset, strerror()
#include <iostream>      // for std::cerr, etc.

#include "./ServerSocket.h"

extern "C" {
  #include "libhw1/CSE333.h"
}

using std::string;

namespace hw4 {

ServerSocket::ServerSocket(uint16_t port) {
  port_ = port;
  listen_sock_fd_ = -1;
}

ServerSocket::~ServerSocket() {
  // Close the listening socket if it's not zero.  The rest of this
  // class will make sure to zero out the socket if it is closed
  // elsewhere.
  if (listen_sock_fd_ != -1)
    close(listen_sock_fd_);
  listen_sock_fd_ = -1;
}

bool ServerSocket::BindAndListen(int ai_family, int *listen_fd) {
  // Use "getaddrinfo," "socket," "bind," and "listen" to
  // create a listening socket on port port_.  Return the
  // listening socket through the output parameter "listen_fd_value".

  // STEP 1:
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family = AF_INET6;       // IPv6 (also handles IPv4 clients)
  hints.ai_socktype = SOCK_STREAM;  // stream
  hints.ai_flags = AI_PASSIVE;      // use wildcard "in6addr_any" address
  hints.ai_flags |= AI_V4MAPPED;    // use v4-mapped v6 if no v6 found
  hints.ai_protocol = IPPROTO_TCP;  // tcp protocol
  hints.ai_canonname = nullptr;
  hints.ai_addr = nullptr;
  hints.ai_next = nullptr;

  struct addrinfo* result;
  string portStr = std::to_string(port_);

  if (getaddrinfo(NULL, portStr.c_str(), &hints, &result) != 0) {
    return false;
  }

  // loop through return addrinfo to create socket
  int listen_fd_value = -1;
  for (auto rptr = result; rptr != nullptr; rptr = rptr->ai_next) {
    listen_fd_value = socket(rptr->ai_family, rptr->ai_socktype,
                                           rptr->ai_protocol);
    if (listen_fd_value == -1) {  // if this socket fail, move on to next one
      continue;
    }

    // socket config
    int optval = 1;
    setsockopt(listen_fd_value, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
    // if bind successful
    if (bind(listen_fd_value, rptr->ai_addr, rptr->ai_addrlen) == 0) {
      break;
    }

    // otherwise do it again
    close(listen_fd_value);
    listen_fd_value = -1;
  }

  freeaddrinfo(result);
  if (listen_fd_value == -1) {
    return false;
  }

  // set up listening socket, if fail error appears
  if (listen(listen_fd_value, SOMAXCONN) != 0) {
    std::cerr << "Failed to mark socket as listening: ";
    std::cerr << strerror(errno) << std::endl;
    close(listen_fd_value);
    return false;
  }

  listen_sock_fd_ = listen_fd_value;
  *listen_fd = listen_fd_value;
  return true;
}

bool ServerSocket::Accept(int *accepted_fd,
                          std::string *client_addr,
                          uint16_t *client_port,
                          std::string *client_dnsname,
                          std::string *server_addr,
                          std::string *server_dnsname) {
  // Accept a new connection on the listening socket listen_sock_fd_.
  // (Block until a new connection arrives.)  Return the newly accepted
  // socket, as well as information about both ends of the new connection,
  // through the various output parameters.

  // STEP 2:
  int client_fd = -1;
  struct sockaddr_storage caddr;
  socklen_t caddr_len = sizeof(caddr);
  struct sockaddr *addr = reinterpret_cast<struct sockaddr *>(&caddr);

  while (1) {
    client_fd =  accept(listen_sock_fd_, addr, &caddr_len);
    // error handling
    if (client_fd < 0) {
      if ((errno == EAGAIN) || (errno == EINTR)) {
        continue;
      }
      return false;
    }

    *accepted_fd = client_fd;

    // print out ip addr accordingly
    if (addr->sa_family == AF_INET) {
      char astring[INET_ADDRSTRLEN];
      struct sockaddr_in *in4 = reinterpret_cast<struct sockaddr_in *>(&addr);
      inet_ntop(AF_INET, &(in4->sin_addr), astring, INET_ADDRSTRLEN);
      *client_addr = string(astring);
      *client_port = ntohs(in4->sin_port);
    } else {
      char astring[INET6_ADDRSTRLEN];
      struct sockaddr_in6 *in6 = reinterpret_cast<struct sockaddr_in6 *>(addr);
      inet_ntop(AF_INET6, &(in6->sin6_addr), astring, INET6_ADDRSTRLEN);
      *client_addr = string(astring);
      *client_port = ntohs(in6->sin6_port);
    }

    char hostname[1024];
    if (getnameinfo(addr, sizeof(caddr), hostname, 1024, nullptr, 0, 0) != 0) {
      std::cerr << hostname << "[reverse DNS failed]" << std::endl;
    }
    *client_dnsname = string(hostname);

    char serhostname[1024];
    serhostname[0] = '\0';

    if (this->sock_family_ == AF_INET) {
      // The server is using an IPv4 address.
      struct sockaddr_in server;
      socklen_t srvrlen = sizeof(server);
      char addrbuf[INET_ADDRSTRLEN];

      getsockname(client_fd, (struct sockaddr *) &server, &srvrlen);
      inet_ntop(AF_INET, &server.sin_addr, addrbuf, INET_ADDRSTRLEN);
      *server_addr = string(addrbuf);

      getnameinfo((const struct sockaddr *) &server,
                  srvrlen, serhostname, 1024, nullptr, 0, 0);
      *server_dnsname = string(serhostname);
    } else {
      // The server is using an IPv6 address.
      struct sockaddr_in6 server;
      socklen_t srvrlen = sizeof(server);
      char addrbuf[INET6_ADDRSTRLEN];

      getsockname(client_fd, (struct sockaddr *) &server, &srvrlen);
      inet_ntop(AF_INET6, &server.sin6_addr, addrbuf, INET6_ADDRSTRLEN);
      *server_addr = string(addrbuf);

      getnameinfo((const struct sockaddr *) &server,
                  srvrlen, serhostname, 1024, nullptr, 0, 0);
      *server_dnsname = string(serhostname);
    }
    break;
  }

  return true;
}

}  // namespace hw4
