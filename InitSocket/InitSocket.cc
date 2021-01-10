#include "InitSocket.h"


// perror
void print_err(const char * str){
	std::cout << str << std::endl;
	exit(-1);
}

// Socket functions
int Socket(){
	int sockfd = 0;
	// sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
	sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        print_err("sockets::createNonblockingOrDie");
    }
	return sockfd;
}

int Bind(int sockfd, const struct sockaddr_in& address){
	int ret = 0;
	ret = bind(sockfd, static_cast<SA*>((void*)&address), sizeof address);
	if(ret == -1)
	{
		print_err("bind error");
	}
	return ret;
};

int Listen(int socket, int backlog){
	int ret = 0;
	ret = listen(socket, backlog);
	if(ret == -1)
	{
		print_err("listen error");
	}
	return ret;
}

ssize_t Read(int fd, void *buf, size_t nbyte){
	ssize_t ret = 0;

again:
	ret = read(fd, buf, nbyte);
	if(ret == -1)
	{
		if(errno == EINTR)
		{
			goto again;
		}
		else
		{
			print_err("read error");
		}
	}
	return ret;
}

