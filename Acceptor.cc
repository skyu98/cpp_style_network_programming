#include "02_poll/server.h"
#include "Acceptor.h"
void defaultConnCallback(int fd, const struct sockaddr_in& addr)
{ 
    char client_ip[INET_ADDRSTRLEN];
    std::cout << "New conn from ip: "
              << inet_ntop(AF_INET, (void *)&addr.sin_addr, client_ip, sizeof(client_ip)) 
              << " port: " << addr.sin_port
              << " at fd " << fd
              << std::endl;
}

Acceptor::Acceptor(Server* server, int port)
    :port_(port),
     listen_fd_(Socket()),
     server_(server),
     connCallback_(defaultConnCallback)
{
    InitSocket();
}

Acceptor::~Acceptor()
{
    close(listen_fd_);
}


void Acceptor::InitSocket()
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

void Acceptor::enableListening()
{
    server_->poller_->addFd(listen_fd_, POLLIN);
}

int Acceptor::accept()
{
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof client_addr;
    int newConnFd = accept4(listen_fd_, 
                            static_cast<SA*>((void*)&client_addr), 
                            &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if(newConnFd < 0)
        print_err("accept error");
    
    connCallback_(newConnFd, client_addr);
    return newConnFd;
}