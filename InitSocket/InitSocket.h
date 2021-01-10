#ifndef INITSOCKET_H
#define INITSOCKET_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

typedef struct sockaddr SA;

void print_err(const char * str);

int Socket();

int Bind(int sockfd, const struct sockaddr_in& address);

int Listen(int socket, int backlog = 128);

ssize_t Read(int fd, void *buf, size_t nbyte);

#endif