#ifndef SERVER_H
#define SERVER_H

#include <memory>

#include "../Acceptor.h"
#include "poller.h"
#include "../InitSocket/InitSocket.h"
#include "../Callbacks.h"
#include <strings.h>

class Acceptor;
class Poller;

class Server{
    friend class Poller;
    friend class Acceptor;
public:
    explicit Server(int port = 8888);
    ~Server();
    void start();

private:
    void startLoop();
    
    int port_;
    std::unique_ptr<Poller> poller_;
    std::unique_ptr<Acceptor> acceptor_;
    messageCallback msgCallback_;
    connectionCallback connCallback_;
    closeCallback clsCallback_;
};

#endif