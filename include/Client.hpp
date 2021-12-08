#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <netdb.h>
# include <poll.h>

# define FDNUM			2
# define BUFFERSIZE		512
# define POLLTIMEOUT	1000
# define STDIN			0

class	Client
{
public:
	Client(std::string hostname, std::string port, std::string nick, std::string user);
	~Client(void);

private:
	std::string		_hostname;
	std::string		_port;
	std::string		_ip;
	int				_fd;

	std::string		_nickname;
	std::string		_username;

	void			_connectToSocket(void);
	void			_getAddrInfoList(struct addrinfo *hints, struct addrinfo **res0);
	void			_displayIpAddress(struct addrinfo *res);

	struct pollfd	_pollfds[FDNUM];
	int				_pollTimeout;
	void			_initPoll(void);
	void			_loop(void);

	std::string		_inputBuffer;
	void			_checkConsoleInput(void);
	void			_checkNetworkInput(void);

	void			_doAutoPong(std::string data);
	void			_doAutoIdent(void);
	void			_sendLine(std::string data);
	void			_displayLine(std::string &line);

	Client(void);
};

#endif
