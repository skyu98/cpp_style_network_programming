#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include "Callbacks.h"
#include <memory>
#include "InitSocket/InitSocket.h"

class Server;

class Acceptor
{
public:
    Acceptor(Server* server, int port);
    ~Acceptor();

    void InitSocket();
    void enableListening();

    void setConnCallback(connectionCallback& cb)
    {
        connCallback_ = cb;
    }
     
    int accept();
private:
    int listen_fd_;
    int port_;
    Server* server_;
    connectionCallback connCallback_;
};


#endif