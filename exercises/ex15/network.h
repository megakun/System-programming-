// ex15
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 08/10/20
// Copyright[2019]<Haokun Cai>

#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <sys/socket.h>

bool LookupName(char *name,
                unsigned short port,
                struct sockaddr_storage *ret_addr,
                size_t *ret_addrlen);

bool Connect(const struct sockaddr_storage &addr,
             const size_t &addrlen,
             int *ret_fd);

// utility funtion for error
int ReadWrapper(int fd, char* buf, int readlen);

int WriteWrapper(int fd, char* buf, int writelen);

#endif  // _NETWORK_H_
