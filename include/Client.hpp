#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <netdb.h>

class	Client
{
public:
	Client(std::string hostname, std::string port);
	~Client(void);

private:
	std::string		_hostname;
	std::string		_port;
	int				_fd;

	struct addrinfo	_hints, *_res, *_res0;
	void			_getAddrInfoList(void);

	void			_connectToSocket(void);

	Client(void);
};

#endif
