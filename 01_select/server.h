#ifndef SERVER_H
#define SERVER_H

#include <memory>

#include "selector.h"
#include "../InitSocket/InitSocket.h"
#include <strings.h>

class Server{
public:
    explicit Server(int port = 8888);
    ~Server();
    void start();

private:
    void startLoop();
    void InitSocket();
    int port_;
    int listen_fd_;
    std::unique_ptr<Selector> selector_;
    messageCallback msgCallback_;
    connectionCallback connCallback_;
    closeCallback clsCallback_;
};

#endif