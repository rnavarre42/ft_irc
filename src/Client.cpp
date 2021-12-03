#include "Client.hpp"
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>

void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

Client::Client(std::string hostname, std::string port)
	: _hostname(hostname)
	, _port(port)
{
	char	dst[INET_ADDRSTRLEN];

	this->_getAddrInfoList();
	this->_connectToSocket();

//	std::cout << "ai_family: " << this->_res->ai_family << std::endl;
	inet_ntop(this->_res->ai_family, get_in_addr((struct sockaddr *)this->_res->ai_addr), dst, INET_ADDRSTRLEN);
	std::cout << "ipAddress: " << dst << std::endl;

//	freeaddrinfo(this->_res0);
}

Client::~Client(void)
{}

void	Client::_getAddrInfoList(void)
{
	int	error;

	memset(&this->_hints, 0, sizeof(this->_hints));
	this->_hints.ai_family = AF_UNSPEC;
	this->_hints.ai_socktype = SOCK_STREAM;
	error = getaddrinfo(this->_hostname.c_str(), this->_port.c_str(), &this->_hints, &this->_res0);
	if (error)
	{
		std::cerr << "Client: getaddrinfo failed: " << gai_strerror(error) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Client::_connectToSocket(void)
{
	int	it = 0;

	for (this->_res = this->_res0; this->_res != NULL; this->_res = this->_res->ai_next)
	{
		it++;
		this->_fd = socket(this->_res->ai_family, this->_res->ai_socktype, this->_res->ai_protocol);
		if (this->_fd < 0)
		{
			std::cerr << "Client: Socket failed" << std::endl;
			continue ;
		}
		if (connect(this->_fd, this->_res->ai_addr, this->_res->ai_addrlen) < 0)
		{
//			std::cerr << "Client: Connect failed" << std::endl;
			close(this->_fd);
			continue ;
		}
		break ;
	}
	if (this->_fd < 0 || !this->_res)
	{
		std::cerr << "Client: failed to connect" << std::endl;
		exit(EXIT_FAILURE);
	}
}
