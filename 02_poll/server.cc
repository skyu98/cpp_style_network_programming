#include "server.h"

Server::Server(int port)
  : port_(port),
    poller_(new Poller(this, -1)),
    acceptor_(new Acceptor(this, port))
{
    std::cout << "server" << std::endl;
}

Server::~Server()
{
    
}

void Server::start()
{
    startLoop();
}

void Server::startLoop()
{
    acceptor_->enableListening();
    poller_->start();
}


