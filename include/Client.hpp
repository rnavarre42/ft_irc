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
	Client(std::string hostname, std::string port, std::string nick, std::string user);
	~Client(void);

private:
	std::string		_hostname;
	std::string		_port;
	std::string		_ip;
	int				_fd;

	std::string		_nickname;
	std::string		_username;

	struct addrinfo	_hints, *_res, *_res0;
	void			_connectToSocket(void);
	void			_getAddrInfoList(void);
	void			_displayIpAddress(void);

	struct pollfd	_pollfds[FDNUM];
	int				_pollTimeout;
	void			_initPoll(void);
	void			_loop(void);

	std::string		_inputBuffer;
	std::string		_outputBuffer;
	void			_checkConsoleInput(void);
	void			_checkNetworkInput(void);

	void			_autoIdent(void);
	void			_send(std::string data);

	Client(void);
};

#endif
