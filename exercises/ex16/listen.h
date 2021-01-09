// ex16
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 08/10/20
// Copyright[2019]<Haokun Cai>

#ifndef _LISTEN_H_
#define _LISTEN_H_

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>

// adapt method from lecture example
int  Listen(char *portnum, int *sock_family);

void HandleClient(int c_fd, struct sockaddr *addr, size_t addrlen,
                  int sock_family);

#endif  // _LISTEN_H_
