#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <netdb.h>

class	Client
{
public:
	Client(std::string host, std::string port);
	~Client(void);

private:
	std::string		_serverHost;
	std::string		_serverPort;
	int				_sockfd;
	struct addrinfo	_hints, *_current, *_first;

	Client(void);
};

#endif
