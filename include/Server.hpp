// Clase encargada de manejar todas las conexiones
#ifndef SERVER_HPP
# define SERVER_HPP

# include "ISender.hpp"
# include "EventHandler.hpp"
# include "Source.hpp"
# include "utils.hpp"
//# include "Channel.hpp"
//# include "ACommand.hpp"
//# include "Message.hpp"
# include "log.hpp"
# include <string>
# include <map>
# include <ctime>
# include <vector>
# include <algorithm>
# include <netinet/in.h>
# include <poll.h>

# define LOG_LEVEL LOG_INFO | LOG_WARNING | LOG_ERROR | LOG_FATAL

# define VERSION		"ircServ 0.1"

# define MAXNICK		9
# define MAXCHANNEL		3
# define MAXSERVER		63
# define MAXTOPIC		307
# define MAXLINE		512
# define MAXHOST		63
# define MAXAWAY		200
# define MAXQUIT		255
# define MAXKICK		255
# define MAXREAL		100

# define IDLETIMEOUT	120
# define REGTIMEOUT		15
# define NEXTTIMEOUT	120
# define BUFFERSIZE		512
# define MAXUSERS		3
# define MAXLISTEN		5

# define NEWUSEREVENT		0x000001
# define DELUSEREVENT		0x000002
# define REGUSEREVENT		0x000004
# define NICKEVENT			0x000008
# define QUITEVENT			0x000010
# define JOINEVENT			0x000020
# define PARTEVENT			0x000040
# define KICKEVENT			0x000080
# define CHANMODEEVENT		0x000100
# define NICKMODEEVENT		0x000200
# define CHANTOPICEVENT		0x000400
# define CHANFULLEVENT		0x000800
# define CHANBANEVENT		0x001000
# define CHANKEYEVENT		0x002000
# define AWAYEVENT			0x004000
# define INVITEEVENT		0x008000
# define NEWCHANEVENT		0x010000
# define DELCHANEVENT		0x020000
# define ALREADYEVENT		0x040000
# define MAXCHANEVENT		0x080000
# define ERRCHANEVENT		0x100000
# define NOTINCHANEVENT		0x200000
# define NOTCHANEVENT		0x400000

//hispano:
//	registerTimeout	 30s
//	idleTimeout		120s
//	pingTimeout		120s

class Channel;
class ACommand;
class User;
class Channel;
struct Source;

class Server : public ISender
{
	public:
	~Server(void);

	typedef std::map<std::string, ACommand *>					aCommandMap_type;
	typedef aCommandMap_type::iterator							aCommandMap_iterator;
	typedef EventHandler<int, Source>							eventHandler_type;
	typedef std::map<std::string, Channel *>					channelMap_type;
	typedef channelMap_type::iterator							channelMap_iterator;
	typedef std::map<std::string, User *>						userMap_type;
	typedef userMap_type::iterator								userMap_iterator;
	typedef std::map<std::string, Server *>						serverMap_type;
	typedef userMap_type::iterator								serverMap_iterator;
	typedef std::map<int, User *>								fdMap_type;
	typedef fdMap_type::iterator								fdMap_iterator;
	typedef std::map<std::string, ACommand *>					commandMap_type;
	typedef std::vector<ISender *>								userVector_type;
	typedef userVector_type::iterator							userVector_iterator;

	static void						signalHandler(int sig);
	static Server					&getInstance(void);
	static Server					&getInstance(std::string listenIp, int listenPort, std::string name);
	static void						deleteInstance(void);
	std::string const				&getName(void) const;
	Server::userMap_type			&getUserMap(void);
	Server::channelMap_type			&getChannelMap(void);
	std::string						getMask(void);

	channelMap_iterator				findChannel(std::string &channelName);
	userMap_iterator				findUser(std::string &userName);
	
	void							delChannel(Channel &channel);
	bool							validChannelPrefix(std::string &channelName);

	void							setPass(std::string value);
	std::string const				&getPass(void) const;

	bool							isUser(void);
	bool							isServer(void);
	bool							isOper(void);
	int								getType(void);
	int const						&getFd(void) const;
	bool const						&isRegistered(void) const;
	void							setRegistered(bool value);
	void							setIdleTime(time_t value);

	void							registerUser(Message &message);

	void	start(void);
	ssize_t	send(std::string msg = "");
	ssize_t	send(Message &message);

	void	addToChannel(Message &message);
	void	delFromChannel(Message &message);

	void	registerUser(User &user);
	void	quit(std::string msg);
	
	void	addUser(User &user);
	void	delUser(User &user, std::string text);
	
//	void	killUser(User &user, std::string reason);
	int		count(void);

	inline ACommand	*findCommand(std::string cmd)
	{
		if (this->_commandMap.find(cmd) != this->_commandMap.end())
			return this->_commandMap[cmd];
		return NULL;
	}

	inline userMap_iterator userFind(std::string &userName)
	{
		return this->_userMap.find(strToUpper(userName));
	}

	inline channelMap_iterator channelFind(std::string &channelName)
	{
		return this->_channelMap.find(strToUpper(channelName));
	}

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

	std::string	_ip;
	int			_fd;
	int			_port;
	int			_opt;
	int			_addrlen;
	int			_pollTimeout;

	bool		_registered;
	bool		_stop;

	std::string	_pass;
	std::string	_name;
	int			_type;
	time_t		_idleTime;

	struct sockaddr_in	_address;
	// El motivo de usar una estructura de tamaño fijo es porque en user almacenamos el indice donde se encuentra registrado el fd en pollfds
	// Si usamos un vector y eliminamos cualquier posición, nos obligaría a hacer un cambio a todos los usuarios que tengan un indice superior
	// en pollIndex. Dado que la estructura pollfds consta de tres campos, creo que no es necesario hacer ningun cambio.
	struct pollfd		_pollfds[MAXUSERS + 2];
	static Server		*_instance;

	fdMap_type			_fdMap;
	userMap_type		_userMap;
	channelMap_type		_channelMap;
	commandMap_type		_commandMap;
	eventHandler_type	_eventHandler;

	Source						_source;
	
	int		_findFreePollIndex(void);
	int		_poll(void);
	int		_checkUserConnection(void);

	void	_removeUserFromChannel(Channel &channel, User &user);
	void	_loadCommands(void);
	void	_unloadCommands(void);
	void	_checkConsoleInput(void);
	void	_checkUserIO(void);
	void	_checkTimeout(void);
	void	_checkUserTimeout(User &user);
	void	_closeClients(std::string msg);
	void	_loop(void);
	void	_initSocket(void);
	void	_bind(void);
	void	_listen(void);
	User	*_accept();
};

#endif
