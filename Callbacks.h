#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <functional>
#include <string>
#include "InitSocket/InitSocket.h"

typedef std::function<void(int sockfd, const char* msg, int len)> messageCallback;

typedef std::function<void (int sockfd, const struct sockaddr_in&)> connectionCallback;

typedef std::function<void (int sockfd)> closeCallback;

void defaultMsgCallback(int fd, const char* buf, int len);

void defaultConnCallback(int fd, const struct sockaddr_in& addr);

void defaultClsCallback(int fd);
#endif
