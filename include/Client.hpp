#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <netdb.h>
# include <poll.h>

# include "Readline.hpp"

# define FDNUM			2
# define BUFFERSIZE		512
# define POLLTIMEOUT	1000
# define STDIN			0

class	Client
{
public:

	~Client(void);

	static Client*	getInstance(void);
	static Client*	createInstance(std::string host, std::string port, std::string nick, std::string ident);
	static void		deleteInstance(void);

	void	start(void);

//	Readline		readline;

private:
	std::string		_hostname;
	std::string		_port;
	std::string		_ip;
	int				_fd;

	std::string		_nick;
	std::string		_ident;

	bool			_connectToSocket(void);
	bool			_getAddrInfoList(struct addrinfo *hints, struct addrinfo **res0);
	void			_displayIpAddress(struct addrinfo *res);

	struct pollfd	_pollfds[FDNUM];
	int				_pollTimeout;
	void			_initPoll(void);
	void			_loop(void);
	bool			_running;

	std::string		_inputBuffer;
	void			_checkConsoleInput(void);
	void			_checkNetworkInput(void);

	void			_doAutoPong(std::string data);
	void			_doAutoIdent(void);
	void			_sendLine(std::string &data);
	void			_displayLine(std::string &line);

	static Client*	_instance;

	Client(void);
	Client(const std::string& hostname, const std::string& port, const std::string& nick, const std::string& ident);
};

#endif
