// Clase encargada de manejar todas las conexiones
#ifndef SERVER_HPP
# define SERVER_HPP

# include "ISender.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "ACommand.hpp"
# include "EventHandler.hpp"
//# include "Message.hpp"
# include "log.hpp"
# include <string>
# include <map>
# include <ctime>
# include <netinet/in.h>
# include <poll.h>

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

# define IDLETIMEOUT	120
# define REGTIMEOUT		120
# define NEXTTIMEOUT	120
# define BUFFERSIZE		512
# define MAXUSERS		3
# define MAXLISTEN		5


//hispano:
//	registerTimeout	 30s
//	idleTimeout		120s
//	pingTimeout		120s

class User;
class Channel;
class ACommand;
class ISender;

class Server : public ISender
{
	public:
	~Server(void);

	static void						signalHandler(int sig);
	static Server					&getInstance(void);
	static Server					&getInstance(std::string listenIp, int listenPort, std::string name);
	static void						deleteInstance(void);
	std::string const				&getName(void) const;
	std::map<std::string, User *>	&getUserMap(void);
	std::string						getMask(void);

	void							setPass(std::string value);
	std::string const				&getPass(void) const;

	bool							isUser(void);
	bool							isServer(void);
	bool							isOper(void);
	int								getType(void);
	int const						&getFd(void) const;
	bool const						&isRegistered(void) const;
	void							setIdleTime(time_t value);

	void	start(void);
	ssize_t	send(std::string msg = "");
	ssize_t	send(Message &message);

	Channel	*addToChannel(std::string name, User &user, int &flags);
	int		delFromChannel(std::string name, User &user);

	void	quit(std::string msg);
	
	void	addUser(User &user);
	void	delUser(User &user);
	
//	void	killUser(User &user, std::string reason);
	int		count(void);

	ACommand	*findCommand(std::string cmd);

	bool		recvCommand(Message &msg);
	bool		sendCommand(Message &msg);

	void	setPollout(User &user);

	struct ServerFullException : public std::exception
	{
		virtual const char	*what(void) const throw();
	};

private:
	Server(std::string listenIp, int listenPort, std::string name);
	Server(void);

	std::string	ip;
	int			fd;
	int			port;
	int			opt;
	int			addrlen;
	int			pollTimeout;

	bool		registered;
	bool		stop;

	std::string	pass;
	std::string	name;
	int			type;
	time_t		idleTime;

	struct sockaddr_in	address;
	// El motivo de usar una estructura de tamaño fijo es porque en user almacenamos el indice donde se encuentra registrado el fd en pollfds
	// Si usamos un vector y eliminamos cualquier posición, nos obligaría a hacer un cambio a todos los usuarios que tengan un indice superior
	// en pollIndex. Dado que la estructura pollfds consta de tres campos, creo que no es necesario hacer ningun cambio.
	struct pollfd		pollfds[MAXUSERS + 2];
	static Server		*instance;

//	std::vector<User *>						userVector;
	std::map<int, User *>					fdMap;
	std::map<std::string, User *>			userMap;
	std::map<std::string, Channel *>		channelMap;
	std::map<std::string, ACommand *>		commandMap;

	EventHandler		eventHandler;

	int		findFreePollIndex(void);
	int		_poll(void);
	int		checkUserConnection(void);

	void	_loadCommands(void);
	void	_unloadCommands(void);
	void	checkConsoleInput(void);
	void	checkUserIO(void);
	void	checkTimeout(void);
	void	checkUserTimeout(User &user);
	void	closeClients(std::string msg);
	void	loop(void);
	void	initSocket(void);
	void	_bind(void);
	void	_listen(void);
	User	*_accept();
};

#endif
