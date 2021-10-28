#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

#include "ISender.hpp"
#include "ACommand.hpp"
#include "log.hpp"
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <netinet/in.h>
#include <poll.h>

# define LOG_LEVEL LOG_INFO | LOG_WARNING | LOG_ERROR | LOG_FATAL

# define MAXNICK		9
# define MAXCHANNEL		50
# define MAXSERVER		63
# define MAXTOPIC		307
# define MAXLINE		512
# define MAXHOST		63
# define MAXAWAY		200
# define MAXQUIT		255
# define MAXKICK		255
# define MAXREAL		100

# define BUFFERSIZE		512
# define MAXUSERS		3
# define MAXLISTEN		5

class User;
class Channel;
class CommandBase;

class Server : public ISender
{
public:
	~Server(void);

	static void			signalHandler(int sig);
	static Server		&getInstance(void);
	static Server		&getInstance(std::string ip, int port);
	std::string const	&getName(void) const;
	int const			&getFd(void) const;

	void	start(void);
	ssize_t	send(std::string msg);
	void	quit(std::string msg);
	int		count(void);

	struct ServerFullException : public std::exception
	{
		virtual const char	*what(void) const throw();
	};

private:
	Server(std::string ip, int port);
	Server(void);

	std::string	ip;
	int			fd;
	int			port;
	int			opt;
	int			addrlen;
	int			timeout;
		
	bool		stop;

	std::string	name;

	struct sockaddr_in	address;
	struct pollfd		pollfds[MAXUSERS + 2];
	static Server		*instance;

//	std::vector<User *>						userVector;
	std::map<int, User *>					fdMap;
	std::map<std::string, User *>			userMap;
	std::map<std::string, Channel *>		channelMap;
	std::map<std::string, CommandBase *>	commandMap;

	int		findFreePollIndex(void);
	int		_poll(void);
	int		checkUserConnection(void);

	void	checkConsoleInput(void);
	void	checkUserInput(void);
	void	closeClients(std::string msg);
	void	loop(void);
	void	initSocket(void);
	void	_bind(void);
	void	_listen(void);
	void	_addUser(User &user);
	void	_delUser(User &user);
	User	*_accept();
};

#endif
