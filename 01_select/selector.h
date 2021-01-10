#ifndef SELECTOR_H
#define SELECTOR_H
#include <sys/select.h>
#include <functional>
#include <string>
#include "../InitSocket/InitSocket.h"

typedef std::function<void(int sockfd, const char* msg, int len)> messageCallback;

typedef std::function<void (int sockfd, const struct sockaddr_in&)> connectionCallback;

typedef std::function<void (int sockfd)> closeCallback;

void defaultMsgCallback(int fd, const char* buf, int);
void defaultConnCallback(int fd, const struct sockaddr_in& addr);
void defaultClsCallback(int fd);

class Selector{
public:
    Selector(int listen_fd_, int time_interval = 0);
    ~Selector();
    void start();

    void setMsgCallback(messageCallback& cb)
    {
        msgCallback_ = cb;
    }

    void setConnCallback(connectionCallback& cb)
    {
        connCallback_ = cb;
    }

    void setCloseCallback(closeCallback& cb)
    {
        clsCallback_ = cb;
    }

private:
    void startLoop();

    fd_set all_set_;
    fd_set ret_set_;

    int max_fds_;
    int listen_fd_;
    bool isLooping;
    int time_interval_;

    messageCallback msgCallback_;
    connectionCallback connCallback_;
    closeCallback clsCallback_;
};

#endif