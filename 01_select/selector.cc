#include "selector.h"
struct sockaddr_in;

void defaultMsgCallback(int fd, const char* buf, int len)
{
    std::cout << "Recv "<< len << " bytes from fd " << fd
              << std::endl;
    write(fd, buf, len);
}

void defaultConnCallback(int fd, const struct sockaddr_in& addr)
{ 
    char client_ip[INET_ADDRSTRLEN];
    std::cout << "New conn from ip: "
              << inet_ntop(AF_INET, (void *)&addr.sin_addr, client_ip, sizeof(client_ip)) 
              << " port: " << addr.sin_port
              << " at fd " << fd
              << std::endl;
}

void defaultClsCallback(int fd)
{
    std::cout << "Close conn at fd " << fd
              << std::endl;
}


Selector::Selector(int listen_fd, int time_interval)
  : listen_fd_(listen_fd),
    max_fds_(0),
    isLooping(false),
    time_interval_(time_interval),
    msgCallback_(defaultMsgCallback),
    connCallback_(defaultConnCallback),
    clsCallback_(defaultClsCallback)
{   
    max_fds_ = listen_fd_;
    FD_ZERO(&all_set_);
    FD_SET(listen_fd_, &all_set_);
}

Selector::~Selector()
{
  close(listen_fd_);
}

void Selector::start()
{
  this->startLoop();
}

void Selector::startLoop()
{
  isLooping = true;
  /* int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

       void FD_CLR(int fd, fd_set *set);
       int  FD_ISSET(int fd, fd_set *set);
       void FD_SET(int fd, fd_set *set);
       void FD_ZERO(fd_set *set);
*/
  std::cout << "loop..." << std::endl;
  int numOfEvent = 0;
  while(true)
  {
    ret_set_ = all_set_;
    struct timeval *timeout;
    if(time_interval_ == 0)
    {
      timeout = nullptr;
    }
    else
    {
      timeout->tv_sec = time_interval_;
      timeout->tv_usec = 0;
    }
    
    numOfEvent = select(max_fds_ + 1, &ret_set_, NULL, NULL, timeout);
    if(numOfEvent < 0)
      print_err("select error");

    /* handle the listen_fd */
    if(FD_ISSET(listen_fd_, &ret_set_))
    {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof client_addr;
        int newConnFd = accept4(listen_fd_, 
                                static_cast<SA*>((void*)&client_addr), 
                                &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
        if(newConnFd < 0)
          print_err("accept error");
        
        connCallback_(newConnFd, client_addr);

        if(newConnFd > max_fds_)
          max_fds_ = newConnFd;
        
        FD_SET(newConnFd, &all_set_);

        if(numOfEvent-- == 1)
          continue;
    }

    /* handle the conn_fd */
    int nBytes;
    for(int i = listen_fd_ + 1;i <= max_fds_;++i)
    {
        if(FD_ISSET(i, &ret_set_))
        {
            char buf[BUFSIZ];
            nBytes = Read(i, buf, sizeof(buf));
            if(nBytes == 0)
            {
              close(i);
              FD_CLR(i, &all_set_);
              if(max_fds_ == i)
                max_fds_--;
              clsCallback_(i);
            }
            else if(nBytes > 0)
            {
              msgCallback_(i, buf, nBytes);
            }

            if(numOfEvent-- == 1)
              break;
        }
    }
  }
}
