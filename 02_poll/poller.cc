#include "server.h"
#include "poller.h"

struct sockaddr_in;

void defaultMsgCallback(int fd, const char* buf, int len)
{
    std::cout << "Recv "<< len << " bytes from fd " << fd
              << std::endl;
    write(fd, buf, len);
}

void defaultClsCallback(int fd)
{
    std::cout << "Close conn at fd " << fd
              << std::endl;
}


Poller::Poller(Server* server, int time_interval)
  : server_(server),
    max_index_(-1),
    isLooping_(false),
    time_interval_(time_interval),
    msgCallback_(defaultMsgCallback),
    clsCallback_(defaultClsCallback)
{   
    for(int i = 0;i < OPEN_MAX;++i)
    {
      clients_[i].fd = -1;
    }
}

Poller::~Poller()
{
}

void Poller::start()
{
  this->startLoop();
}

void Poller::startLoop()
{
  isLooping_ = true;
  std::cout << "loop..." << std::endl;
  int numOfEvent = 0;
  while(true)
  {
    numOfEvent = poll(clients_, max_index_ + 1, time_interval_);  // 使用poll阻塞监听（**max_index + 1**)
		if(numOfEvent < 0)
			print_err("poll error");

		if(clients_[0].revents == 1)  // lfd有读事件（说明有客户端连接请求）
		{
			int newConnFd = server_->acceptor_->accept();
      addFd(newConnFd, POLLIN);
     
			if(numOfEvent-- == 1) continue; // 如果只有一个读事件，又确定是lfd，则直接进入下一轮监听
		}

    /* handle the conn_fd */
    int nBytes;
		for(int i = 1;i <= max_index_;++i) // 遍历处理满足读事件的fd
		{
			int socket_fd = clients_[i].fd;
			if(socket_fd < 0)
				continue;

			if(clients_[i].revents == 1)
			{
				/* 注意此处不能使用while，否则在当前client关闭之前，
				 * server会阻塞读，无法处理其他client的请求*/
        char buf[BUFSIZ];
				nBytes = Read(socket_fd, buf, sizeof(buf)); // 读写操作
				if(nBytes == 0)
        {
          close(socket_fd);
          clients_[i].fd = -1;
          clsCallback_(socket_fd);
          if(max_index_ == i)
            max_index_--;
        }
        else if(nBytes > 0)
        {
          msgCallback_(socket_fd, buf, nBytes);
        }

        if(numOfEvent-- == 1)
          break;// 如果只有一个fd，处理后则可以直接返回(注意位置：在处理的if逻辑中才做此操作，不处理就不自减)
			}
		}
	}
}

void Poller::addFd(int fd, int events)
{
  int i = 0;
  for(i = 0;i < OPEN_MAX;++i)
  {
    if(clients_[i].fd == -1)
    {
      clients_[i].fd = fd;
      clients_[i].events = events;
      break;
    }
  }

  if(i > max_index_)
    max_index_ = i;
  
  if(i == OPEN_MAX)
  {
    printf("Too many clients!!!\n");
    return;
  }
}
