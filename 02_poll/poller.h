#ifndef POLLER_H
#define POLLER_H

#define OPEN_MAX 1024
#include <poll.h>
#include "../InitSocket/InitSocket.h"
#include "../Callbacks.h"

// struct pollfd {
//     int   fd;         /* file descriptor */
//     short events;     /* requested events */
//     short revents;    /* returned events */
// };
class Server;

class Poller{
public:
    Poller(Server* server, int time_interval);
    ~Poller();
    void start();

    void setMsgCallback(messageCallback& cb)
    {
        msgCallback_ = cb;
    }

    void setCloseCallback(closeCallback& cb)
    {
        clsCallback_ = cb;
    }

    void addFd(int fd, int events);

private:
    void startLoop();

    Server* server_;
    int max_index_;
    bool isLooping_;
    int time_interval_;
    struct pollfd clients_[OPEN_MAX];
    messageCallback msgCallback_;
    closeCallback clsCallback_;
};

#endif