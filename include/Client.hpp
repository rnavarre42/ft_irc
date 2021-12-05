#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <netdb.h>
# include <poll.h>

# define FDNUM		2
# define BUFFERSIZE	512


class	Client
{
public:
	Client(std::string hostname, std::string port);
	~Client(void);

private:
	std::string		_hostname;
	std::string		_port;
	std::string		_ip;
	int				_fd;

	struct addrinfo	_hints, *_res, *_res0;
	void			_connectToSocket(void);
	void			_getAddrInfoList(void);
	void			_displayIpAddress(void);

	struct pollfd	_pollfds[FDNUM];
	int				_pollTimeout;
	void			_loop(void);

	std::string	_inputBuffer;
	void		_checkConsoleInput(void);
	void		_checkNetworkInput(void);


	Client(void);
};

#endif
