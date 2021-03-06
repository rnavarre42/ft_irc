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
#include <csignal>
#include <poll.h>

void	signalHandler(int sig)
{
	if (sig == SIGINT)
		std::cout << std::endl << "irclient :stopped by user" << std::endl;
	exit(0);
}

Client*	Client::_instance = 0;

Client*	Client::getInstance(void)
{
	return Client::_instance;
}

Client*	Client::createInstance(const std::string host, const std::string port, const std::string nick, const std::string ident)
{
	if (Client::_instance == 0)
		Client::_instance = new Client(host, port, nick, ident);
	return Client::_instance;
}

void	Client::deleteInstance(void)
{
	delete Client::_instance;
	Client::_instance = 0;
}

Client::Client(const std::string& hostname, const std::string& port, const std::string& nick, const std::string& ident)
	: _hostname(hostname)
	, _port(port)
	, _nick(nick)
	, _ident(ident)
	, _running(1)
{
	signal(SIGINT, signalHandler);
}

Client::~Client(void)
{
	close(this->_fd);
}

void	Client::start(void)
{
	if (this->_connectToSocket())
	{
		this->_doAutoIdent();
		this->_loop();
	}
}

void	Client::_doAutoIdent(void)
{
	std::string	registerLine("PASS 1234\r\nUSER " + this->_ident + " 0 * :the last param\r\nNICK " + this->_nick);

	this->_sendLine(registerLine);
}

bool	Client::_getAddrInfoList(struct addrinfo* hints, struct addrinfo** res0)
{
	int	error;

	std::memset(hints, 0, sizeof(struct addrinfo));
	hints->ai_family = AF_UNSPEC;
	hints->ai_socktype = SOCK_STREAM;
	error = getaddrinfo(this->_hostname.c_str(), this->_port.c_str(), hints, res0);
	if (error)
	{
		std::cerr << "irclient :getaddrinfo failed: " << gai_strerror(error) << std::endl;
		return false;
	}
	return true;
}

void	Client::_displayIpAddress(struct addrinfo* res)
{
	char		dst[INET_ADDRSTRLEN];

	inet_ntop(res->ai_family, &((struct sockaddr_in *)res->ai_addr)->sin_addr, dst, INET_ADDRSTRLEN);
	this->_ip = dst;
	std::cout << "connecting to address: " << this->_ip << std::endl;
}

bool	Client::_connectToSocket(void)
{
	struct addrinfo	hints, *res, *res0;
	std::string		strError;

	if (!this->_getAddrInfoList(&hints, &res0))
		return false;
	for (res = res0; res != 0; res = res->ai_next)
	{
		this->_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (this->_fd < 0)
		{
			strError = "socket failed";
			continue ;
		}
		if (connect(this->_fd, res->ai_addr, res->ai_addrlen) < 0)
		{
			strError = "failed to connect";
			close(this->_fd);
			continue ;
		}
		break ;
	}
	if (this->_fd < 0 || !res)
	{
		std::cerr << "irclient :" << strError << std::endl;
		freeaddrinfo(res0);
		return false;
	}
	this->_displayIpAddress(res);
	freeaddrinfo(res0);
	return true;
}

void	Client::_sendLine(std::string& data)
{
	data += "\r\n";
	send(this->_fd, data.c_str(), data.size(), 0);
}

void	Client::_checkConsoleInput(void)
{
	std::string	buffer;

	if (this->_pollfds[0].revents & POLLIN)
	{
//		buffer = readline();
		std::getline(std::cin, buffer);
		if (!std::cin)
		{
			std::cout << "irclient :getline failed" << std::endl;
			this->_running = 0;
			return ;
		}
		this->_sendLine(buffer);
	}
}

void	Client::_doAutoPong(std::string& str)
{
	if (!str.compare(0, 4, "PING"))
	{
		str.replace(0, 4, "PONG");
		this->_sendLine(str);
		this->_displayLine(str);
	}
}

void	Client::_displayLine(std::string& line)
{
	std::cout << line << std::endl;
}

void	Client::_checkNetworkInput(void)
{
	ssize_t		size;
	char		buffer[BUFFERSIZE + 1];
	std::string	line;
	size_t		pos;

	if (this->_pollfds[1].revents & POLLIN)
	{
		size = recv(this->_fd, buffer, BUFFERSIZE, 0);
		if (size == -1)
		{
			std::cout << "irclient :recv failed" << std::endl;
			this->_running = 0;
			return ;
		}
		buffer[size] = '\0';
		this->_inputBuffer += buffer;
		while ((pos = this->_inputBuffer.find('\n')) != std::string::npos)
		{
			line = this->_inputBuffer.substr(0, pos);
			this->_inputBuffer.erase(0, pos + 1);
			while ((pos = line.find('\r')) != std::string::npos)
				line.erase(pos, 1);
			this->_displayLine(line);
			this->_doAutoPong(line);
		}
	}
	if (this->_pollfds[1].revents & POLLHUP)
	{
		std::cout << "irclient :socket has been disconnected, client exit" << std::endl;
		this->_running = 0;
	}
}

void	Client::_initPoll(void)
{
	this->_pollTimeout = POLLTIMEOUT;
	this->_pollfds[0].fd = STDIN;
	this->_pollfds[0].events = POLLIN;
	this->_pollfds[1].fd = this->_fd;
	this->_pollfds[1].events = POLLIN | POLLHUP;
}

void	Client::_loop(void)
{
	int	ret;

	this->_initPoll();
	while (this->_running)
	{
		ret = poll(this->_pollfds, FDNUM, this->_pollTimeout);
		if (ret == -1)
		{
			std::cerr << "irclient :poll failed" << std::endl;
			break ;
		}
		this->_checkConsoleInput();
		this->_checkNetworkInput();
	}
}
