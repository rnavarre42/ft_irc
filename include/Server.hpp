// Clase encargada de manejar todas las conexiones
#ifndef SERVER_HPP
# define SERVER_HPP

# include "ASender.hpp"
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

//	registerTimeout	 30s
//	idleTimeout		120s
//	pingTimeout		120s

class Channel;
class ACommand;
class AChanMode;
class User;
class Unknown;
class Channel;

class Server : public ASender
{
public:
	Server(const Unknown& src);
	~Server(void);

	typedef EventHandler<int, Message>				eventHandler_type;
	typedef std::map<std::string, Channel*>			channelMap_type;
	typedef channelMap_type::iterator				channelMap_iterator;
	typedef channelMap_type::const_iterator			channelMap_const_iterator;
	typedef std::pair<channelMap_iterator, bool>	channelMap_insert;
	typedef std::pair<std::string, Channel*>		stringChannelPair_type;
	typedef std::map<std::string, User*>			userMap_type;
	typedef userMap_type::iterator					userMap_iterator;
	typedef std::pair<userMap_iterator, bool>		userMap_insert;
	typedef std::pair<std::string, User*>			stringUserPair_type;
	typedef std::map<std::string, Server*>			serverMap_type;
	typedef userMap_type::iterator					serverMap_iterator;
	typedef std::map<int, ASender*>					fdMap_type;
	typedef fdMap_type::iterator					fdMap_iterator;
	typedef std::map<std::string, ACommand*>		aCommandMap_type;
	typedef aCommandMap_type::iterator				aCommandMap_iterator;
	typedef std::vector<ASender*>					userVector_type;
	typedef userVector_type::iterator				userVector_iterator;
	typedef std::set<Channel*>						channelSet_type;
	typedef channelSet_type::iterator				channelSet_iterator;
	typedef std::map<char, AChanMode*>				aChanModeMap_type;
	typedef aChanModeMap_type::iterator				aChanModeMap_iterator;

	static void			signalHandler(int sig);
	static Server&		getInstance(void);
	static Server*		createInstance(const std::string& listenIp, int listenPort, const std::string& pass, const std::string& name, const std::string& real);
	static void			deleteInstance(void);
	userMap_type&		getUserMap(void);
	channelMap_type&	getChannelMap(void);
	AChanMode*			findChanMode(char modeChar);

	Invite&	invite(void)
	{
		return this->_invite;
	}

	bool	checkChannelMode(Message& message, int commandEvent);

	void		eraseChannel(Channel& channel);
	Channel*	insertChannel(const std::string& channelName, const User& owner);

	void	eraseUser(User& user);
	void	insertUser(User& user);
	void	insertUnknown(Unknown& unknown);

	void	names(Channel& channel);
	void	removeUserFromChannel(Channel& channel, User& user);

	bool		isUser(void);
	bool		isServer(void);
	bool		isOper(void);
	int			getType(void);
	const int&	getFd(void) const;
	void		setSenderLevel(ASender& sender, int value);
	void		setIdleTime(time_t value);
	const time_t&	getIdleTime(void) const;
	void		setNextTimeout(time_t value);
	const time_t&	getNextTimeout(void) const;
	void		setPingChallenge(const std::string& value);
	const std::string&	getPingChallenge(void) const;

	void	run(void);
	
	ssize_t	send(void);
	ssize_t	send(const Message& message);
	ssize_t	send(const std::string& data);

	void	addToChannel(Message& message);
	void	delFromChannel(Message& message);

	void	registerUser(User& user);
	void	quit(const std::string& msg);
	
	void	createUser(User& user);
	void	deleteUser(ASender& sender, std::string text);
	
	int	count(void);

	bool	recvCommand(Message& msg);
	bool	sendCommand(Message& msg);

	void	setPollout(ASender& sender);
	
	size_t	checkInput(int fd, Message& message);
	bool	checkOutput(int fd);

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
	Server(void);

protected:
	Server(const std::string& listenIp, int listenPort, const std::string& pass, const std::string& name, const std::string& real);
	Server(Server& server, time_t signTime, int status, const std::string& pass, const std::string& name, const std::string& host, const std::string& real);

	std::string	_ip;
	int			_port;
	int			_opt;
	int			_addrlen;
	int			_pollTimeout;
	Message&	_message;

	bool		_stop;

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
	
	int	_freePollIndexFind(void);
	int	_poll(void);
	int	_checkNewConnection(void);
	void	_setSignals(void);
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
	void	_checkConnectionTimeout(ASender& sender);
	void	_closeClients(const std::string& msg);
	void	_loop(void);
	void	_initSocket(void);
	void	_bind(void);
	void	_listen(void);
	Unknown*	_accept();


	void	_updateMask(void);
};

#endif
