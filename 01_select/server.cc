#include "server.h"

Server::Server(int port)
  : port_(port),
    listen_fd_(Socket()),
    selector_(new Selector(listen_fd_))
{
    std::cout << "server" << std::endl;
    InitSocket();
}

Server::~Server()
{
    close(listen_fd_);
}

void Server::start()
{
    startLoop();
}

void Server::startLoop()
{
    selector_->start();
}

void Server::InitSocket()
{
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    Bind(listen_fd_, server_addr);
    Listen(listen_fd_);

    printf("listen fd %d is ok\n", listen_fd_);
}