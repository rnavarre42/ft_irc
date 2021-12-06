#include "Client.hpp"
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <poll.h>

void signal_handler(int sig)
{
	if (sig == SIGINT)
		std::cout << std::endl;
	exit(1);
}

Client::Client(std::string hostname, std::string port)
	: _hostname(hostname)
	, _port(port)
{
	signal(SIGINT, signal_handler);
	this->_connectToSocket();
	this->_autoIdent();
	this->_loop();
}

Client::~Client(void)
{
	close(this->_fd);
}

void	Client::_autoIdent(void)
{
	this->_send("NICK tomasito\r\nUSER tomas * * *");
}

void	Client::_getAddrInfoList(void)
{
	int	error;

	bzero(&this->_hints, sizeof(struct addrinfo));
	this->_hints.ai_family = AF_UNSPEC;
	this->_hints.ai_socktype = SOCK_STREAM;
	error = getaddrinfo(this->_hostname.c_str(), this->_port.c_str(), &this->_hints, &this->_res0);
	if (error)
	{
		std::cerr << "client: getaddrinfo failed: " << gai_strerror(error) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Client::_displayIpAddress(void)
{
	char		dst[INET_ADDRSTRLEN];

	inet_ntop(this->_res->ai_family, &((struct sockaddr_in *)this->_res->ai_addr)->sin_addr, dst, INET_ADDRSTRLEN);
	this->_ip = dst;
	std::cout << "connecting to address: " << this->_ip << std::endl;
}

void	Client::_connectToSocket(void)
{
	std::string	strError;

	this->_getAddrInfoList();
	for (this->_res = this->_res0; this->_res != NULL; this->_res = this->_res->ai_next)
	{
		this->_fd = socket(this->_res->ai_family, this->_res->ai_socktype, this->_res->ai_protocol);
		if (this->_fd < 0)
		{
			strError = "socket failed";
			continue ;
		}
		if (connect(this->_fd, this->_res->ai_addr, this->_res->ai_addrlen) < 0)
		{
			strError = "failed to connect";
			close(this->_fd);
			continue ;
		}
		break ;
	}
	if (this->_fd < 0 || !this->_res)
	{
		std::cerr << "client: " << strError << std::endl;
		exit(EXIT_FAILURE);
	}
	this->_displayIpAddress();
	freeaddrinfo(this->_res0);
}

void	Client::_send(std::string data)
{
	data += "\r\n";
	::send(this->_fd, data.c_str(), data.size(), 0);
}

void	Client::_checkConsoleInput(void)
{
	ssize_t	size;
	char	buffer[BUFFERSIZE + 1];

	if (this->_pollfds[0].revents & POLLIN)
	{
		size = read(0, buffer, BUFFERSIZE);
		if (size == -1)
		{
			std::cout << "client: read failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		buffer[size] = '\0';
		::send(this->_fd, buffer, strlen(buffer) - 1, 0);
		::send(this->_fd, "\r\n", 2, 0);
	}
}

void	Client::_checkNetworkInput(void)
{
	ssize_t		size;
	char		buffer[BUFFERSIZE + 1];
	std::string	line;
	size_t		pos;
	
	if (this->_pollfds[1].revents & POLLHUP)
	{
		std::cout << "Socket has been disconnected, goodbye!" << std::endl;
		exit(0);
	}
	else if (this->_pollfds[1].revents & POLLIN)
	{
		size = recv(this->_fd, buffer, BUFFERSIZE, 0);
		if (size == -1)
		{
			std::cout << "client: recv failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		buffer[size] = '\0';
		this->_inputBuffer += buffer;
		while ((pos = this->_inputBuffer.find('\n')) != std::string::npos)
		{
			line = this->_inputBuffer.substr(0, pos);
			this->_inputBuffer.erase(0, pos + 1);
			while ((pos = line.find('\r')) != std::string::npos)
				line.erase(pos, 1);
			std::cout << line << std::endl;
			if (!line.compare(0, 4, "PING"))
			{
				line.replace(0, 4, "PONG");
				this->_send(line);
			}
		}
	}
}

void	Client::_initPoll(void)
{
	this->_pollTimeout = 1000;
	this->_pollfds[0].fd = 0;
	this->_pollfds[0].events = POLLIN;
	this->_pollfds[1].fd = this->_fd;
	this->_pollfds[1].events = POLLIN | POLLHUP;
}

void	Client::_loop(void)
{
	int	rv;

	this->_initPoll();
	while (1)
	{	
		rv = poll(this->_pollfds, FDNUM, this->_pollTimeout);
		if (rv == -1)
		{
			std::cerr << "client: poll failed" << std::endl;
			exit(EXIT_FAILURE);
		}
		this->_checkConsoleInput();
		this->_checkNetworkInput();
	}
}
