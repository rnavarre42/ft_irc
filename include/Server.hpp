// Clase encargada de manejar todas las conexiones
#ifndef SERVER_HPP
# define SERVER_HPP

# include "ISender.hpp"
# include "EventHandler.hpp"
# include "utils.hpp"
# include "log.hpp"
# include "Invite.hpp"

# include <string>
# include <map>
# include <ctime>
# include <vector>
# include <set>
# include <algorithm>
# include <netinet/in.h>
# include <poll.h>

# define LOG_LEVEL LOG_INFO | LOG_WARNING | LOG_ERROR | LOG_FATAL

# define VERSION			"ircServ-0.1"
# define SHUTDOWN_STRING	"Server shutting down"

# define OPERPASSWORD		"1234"
# define OPERLOGIN			"laura"

# define MAXNICK			9
# define MAXUSER			9
# define MAXCHANNEL			10
# define CHANNELLEN			64
# define MAXSERVER			63
# define MAXKEY				32
# define MAXTOPIC			307
# define MAXLINE			512
# define MAXHOST			63
# define MAXAWAY			200
# define MAXQUIT			255
# define MAXKICK			255
# define MAXREAL			100

# define IDLETIMEOUT		30
# define REGTIMEOUT			30
# define NEXTTIMEOUT		120
# define BUFFERSIZE			512
# define MAXUSERS			10
# define MAXLISTEN			5

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

class Channel;
class ACommand;
class AChanMode;
class User;
class Channel;

class Server : public ISender
{
public:
	~Server(void);

	typedef EventHandler<int, Message>				eventHandler_type;
	typedef std::map<std::string, Channel* >		channelMap_type;
	typedef channelMap_type::iterator				channelMap_iterator;
	typedef std::pair<channelMap_iterator, bool>	channelMap_insert;
	typedef std::pair<std::string, Channel* >		stringChannelPair_type;
	typedef std::map<std::string, User* >			userMap_type;
	typedef userMap_type::iterator					userMap_iterator;
	typedef std::pair<userMap_iterator, bool>		userMap_insert;
	typedef std::pair<std::string, User *>			stringUserPair_type;
	typedef std::map<std::string, Server* >			serverMap_type;
	typedef userMap_type::iterator					serverMap_iterator;
	typedef std::map<int, User* >					fdMap_type;
	typedef fdMap_type::iterator					fdMap_iterator;
	typedef std::map<std::string, ACommand* >		aCommandMap_type;
	typedef aCommandMap_type::iterator				aCommandMap_iterator;
	typedef std::vector<ISender* >					userVector_type;
	typedef userVector_type::iterator				userVector_iterator;
	typedef std::set<Channel* >						channelSet_type;
	typedef channelSet_type::iterator				channelSet_iterator;
	typedef std::map<char, AChanMode* >				aChanModeMap_type;
	typedef aChanModeMap_type::iterator				aChanModeMap_iterator;
	typedef std::map<std::string, std::string>		supportMap_type;
	typedef supportMap_type::iterator				supportMap_iterator;

	static void			signalHandler(int sig);
	static Server&		getInstance(void);
	static Server*		createInstance(const std::string& listenIp, int listenPort, const std::string& name, const std::string& password, const std::string& realName);
	static void			deleteInstance(void);
	const std::string&	getName(void) const;
	userMap_type&		getUserMap(void);
	void				setReal(const std::string& value);
	const std::string&	getReal(void) const;
	channelMap_type&	getChannelMap(void);
	const std::string&	getMask(void) const;
	AChanMode*			findChanMode(char modeChar);

	Invite&	invite(void)
	{
		return this->_invite;
	}

	bool	checkChannelMode(Message& message, int commandEvent);

	void		eraseChannel(Channel& channel);
	Channel*	insertChannel(const std::string& channelName, const User& owner);

	void	eraseUser(User& user);
	void	insertUser(User* user);

	void				 setPass(const std::string& value);
	const std::string&	getPass(void) const;

	void	chanModeNames(Channel& channel);
	void	supportNames(void);
	void	names(Channel& channel);
	void	removeUserFromChannel(Channel& channel, User& user);

	bool		isUser(void);
	bool		isServer(void);
	bool		isOper(void);
	int			getType(void);
	const int&	getFd(void) const;
	int			getStatus(void);
	void		setStatus(int value);
	void		setSenderStatus(ISender& sender, int value);
	void		setIdleTime(time_t value);

	void	run(void);
	ssize_t	send(void);
	ssize_t	send(const std::string& data);
	ssize_t	send(const Message& message);

	void	addToChannel(Message& message);
	void	delFromChannel(Message& message);

	void	registerUser(User& user);
	void	quit(const std::string& msg);
	
	void	createUser(User& user);
	void	deleteUser(User& user, const std::string& text);
	
	int	count(void);

	bool	recvCommand(Message& msg);
	bool	sendCommand(Message& msg);

	void	setPollout(User& user);

	bool	isChannel(const std::string& channelName)
	{
		return channelName[0] == '#';
	}

	ACommand*	commandFind(const std::string& cmd)
	{
		if (this->_commandMap.find(cmd) != this->_commandMap.end())
			return this->_commandMap[cmd];
		return NULL;
	}

	userMap_iterator	userFind(const std::string& userName)
	{
		return this->_userMap.find(strToUpper(userName));
	}

	channelMap_iterator	channelFind(const std::string& channelName)
	{
		return this->_channelMap.find(strToUpper(channelName));
	}

	Channel*			channelAt(const std::string& channelName)
	{
		Server::channelMap_iterator	it;

		if ((it = this->channelFind(channelName)) == this->_channelMap.end())
			return 0;
		return it->second;
	}

	User*				userAt(const std::string& userName)
	{
		Server::userMap_iterator it;

		if ((it = this->userFind(userName)) == this->_userMap.end())
			return 0;
		return it->second;
	}

	struct	NotImplementedException : public std::exception
	{
		const char*	what(void) const throw()
		{
			return "Functionality not implemented yet";
		}
	};

private:
	Server(const std::string& listenIp, int listenPort, const std::string& name, const std::string& password, const std::string& realName);
	Server(void);

	std::string	_ip;
	int			_fd;
	int			_port;
	int			_opt;
	int			_addrlen;
	int			_pollTimeout;
	Message&	_message;

	int			_status;
	bool		_stop;

	std::string	_pass;
	std::string	_name;
	std::string	_real;
	int			_type;
	time_t		_idleTime;

	struct sockaddr_in	_address;
	// El motivo de usar una estructura de tamaño fijo es porque en user almacenamos el indice donde se encuentra registrado el fd en pollfds
	// Si usamos un vector y eliminamos cualquier posición, nos obligaría a hacer un cambio a todos los usuarios que tengan un indice superior
	// en pollIndex. Dado que la estructura pollfds consta de tres campos, creo que no es necesario hacer ningun cambio.
	struct pollfd	_pollfds[MAXUSERS + 2];

	static Server*	_instance;

	fdMap_type			_fdMap;
	userMap_type		_userMap;
	channelMap_type		_channelMap;
	aCommandMap_type	_commandMap;
	eventHandler_type	_eventHandler;
	aChanModeMap_type	_chanModeMap;
	Invite				_invite;
	supportMap_type		_supportMap;
	
	int		_freePollIndexFind(void);
	int		_poll(void);
	int		_checkUserConnection(void);
	void	_setSignals(void);
	void	_loadSupport(void);
	void	_loadCommands(void);
	void	_unloadCommands(void);
	void	_unloadChanModes(void);
	bool	_unloadChanMode(char modeName);
	bool	_unloadChanMode(aChanModeMap_iterator it);
	void	_loadChanModes(void);
	void	_loadChanMode(AChanMode* newChanMode);
	void	_checkConsoleInput(void);
	void	_checkUserIO(void);
	void	_checkTimeout(void);
	void	_checkUserTimeout(User& user);
	void	_closeClients(const std::string& msg);
	void	_loop(void);
	void	_initSocket(void);
	void	_bind(void);
	void	_listen(void);
	User*	_accept();
};

#endif
