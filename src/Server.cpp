#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Console.hpp"
#include "Message.hpp"
#include "usermodes.hpp"
#include "chanmodes.hpp"
#include "commands.hpp"
#include "Numeric.hpp"
#include "ChanModeConfig.hpp"
#include "utils.hpp"

#include <cerrno>
#include <deque>
#include <set>
#include <string>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <exception>
#include <csignal>
#include <cstdlib>
#include <sstream>

Server*	Server::_instance = NULL;

Server::Server(const std::string& listenIp, int listenPort, const std::string& name, const std::string& password, const std::string& realName)
	: _ip(listenIp)
	, _port(listenPort)
	, _pollTimeout(1000)
	, _message(*new Message(*this))
	, _status(0)
	, _stop(false)
	, _pass(password)
	, _name(name)
	, _real(realName)
	, _type(TYPE_SERVER)
{
	this->_loadSupport();
	this->_loadCommands();
	this->_loadChanModes();
	this->_loadUserModes();
	this->_setSignals();
	std::memset(this->_pollfds, '\0', sizeof(struct pollfd) * (MAXUSERS + 2));
	this->_initSocket();
	this->_bind();
}

Server::~Server(void)
{
	this->_unloadCommands();
	this->_unloadChanModes();
	delete &this->_message;
	Console::log(LOG_INFO, "Server destroyed");
}

void	Server::signalHandler(int sig)
{
	Server &server = Server::getInstance();

	std::cout << std::endl;
	if (sig == SIGINT)
		server.quit(SHUTDOWN_STRING);
}

void	Server::_loadSupport(void)
{
	this->_supportMap["AWAYLEN"]	= "200";
	this->_supportMap["CHANLIMIT"]	= "10";
	this->_supportMap["CHANMODES"]	= "be,k,,inst";
	this->_supportMap["CHANNELLEN"]	= "64";
	this->_supportMap["EXCEPTS"]	= "e";
	this->_supportMap["KEYLEN"]		= "32";
	this->_supportMap["LINELEN"]	= "512";
	this->_supportMap["NETWORK"]	= "IRC-Castela";
	this->_supportMap["NICKLEN"]	= "9";
	this->_supportMap["PREFIX"]		= "(ov)@+";
	this->_supportMap["STATUSMSG"]	= "@+";
	this->_supportMap["TOPICLEN"]	= "307";
	this->_supportMap["USERMODES"]	= "io";
	this->_supportMap["MODES"]		= "6";
	this->_supportMap["USERLEN"]	= "9";
}

void	Server::_loadCommands(void)
{

	this->_commandMap["AWAY"]		= new AwayCommand	  (*this, LEVEL_REGISTERED, 0);
	this->_commandMap["INVITE"]		= new InviteCommand	  (*this, LEVEL_REGISTERED, 2);
	this->_commandMap["JOIN"]		= new JoinCommand	  (*this, LEVEL_REGISTERED, 1);
	this->_commandMap["KICK"]		= new KickCommand	  (*this, LEVEL_REGISTERED, 2);
	this->_commandMap["KILL"]		= new KillCommand	  (*this, LEVEL_IRCOPERATOR, 2);
	this->_commandMap["LIST"]		= new ListCommand	  (*this, LEVEL_REGISTERED, 0);
	this->_commandMap["MODE"]		= new ModeCommand	  (*this, LEVEL_REGISTERED, 1);
	this->_commandMap["MOTD"]		= new MotdCommand	  (*this, LEVEL_REGISTERED, 0);
	this->_commandMap["NAMES"]		= new NamesCommand	  (*this, LEVEL_REGISTERED, 1);
	this->_commandMap["NICK"]		= new NickCommand	  (*this, LEVEL_ALL, 1);
	this->_commandMap["NOTICE"]		= new NoticeCommand	  (*this, LEVEL_REGISTERED, 2);
	this->_commandMap["OPER"]		= new OperCommand	  (*this, LEVEL_REGISTERED, 2);
	this->_commandMap["PART"]		= new PartCommand	  (*this, LEVEL_REGISTERED, 1);
	this->_commandMap["PASS"]		= new PassCommand	  (*this, LEVEL_UNREGISTERED, 1);
	this->_commandMap["PING"]		= new PingCommand	  (*this, LEVEL_REGISTERED, 1);
	this->_commandMap["PONG"]		= new PongCommand	  (*this, LEVEL_NEGOTIATING | LEVEL_REGISTERED, 1);
	this->_commandMap["PRIVMSG"]	= new PrivmsgCommand  (*this, LEVEL_REGISTERED, 1);
	this->_commandMap["QUIT"]		= new QuitCommand	  (*this, LEVEL_ALL, 0);
	this->_commandMap["SHUTDOWN"]	= new ShutdownCommand (*this, LEVEL_IRCOPERATOR, 1);
	this->_commandMap["TOPIC"]		= new TopicCommand	  (*this, LEVEL_REGISTERED, 1);
	this->_commandMap["USER"]		= new UserCommand	  (*this, LEVEL_UNREGISTERED, 4);
	this->_commandMap["WHOIS"]		= new WhoisCommand	  (*this, LEVEL_REGISTERED, 1);

//	this->_commandMap["WHO"]		= new WhoCommand	(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["WHOWAS"]		= new WhowasCommand	(*this, LEVEL_REGISTERED, 1);

	Server::commandMap_iterator	it;

	for (it = this->_commandMap.begin(); it != this->_commandMap.end(); it++)
		it->second->loadEvents(this->_eventHandler);
}

void	Server::_unloadCommands(void)
{
	for (Server::commandMap_iterator it = this->_commandMap.begin(); it != this->_commandMap.end(); it++)
	{
		it->second->unloadEvents(this->_eventHandler);
		delete it->second;
	}

	for (Server::eventHandler_type::delegateMultimap_iterator it = this->_eventHandler.begin(); it != this->_eventHandler.end(); it++)
		delete &it->second;
}

void	Server::_loadChanModes(void)
{
	this->_loadChanMode(new BanChanMode(*this));
	this->_loadChanMode(new ExceptChanMode(*this));
	this->_loadChanMode(new InviteChanMode(*this));
	this->_loadChanMode(new KeyChanMode(*this));
	this->_loadChanMode(new LimitChanMode(*this));
	this->_loadChanMode(new ModerateChanMode(*this));
	this->_loadChanMode(new NoExternalChanMode(*this));
	this->_loadChanMode(new OperChanMode(*this));
	this->_loadChanMode(new SecretChanMode(*this));
	this->_loadChanMode(new TopicChanMode(*this));
	this->_loadChanMode(new VoiceChanMode(*this));
}

void	Server::_loadChanMode(AChanMode* newChanMode)
{
	const ChanModeConfig&	chanModeConfig = newChanMode->getConfig();

	this->_chanModeMap[chanModeConfig.mode] = newChanMode;
}

void	Server::_unloadChanModes(void)
{
	Server::chanModeMap_iterator	currentIt;

	for (Server::chanModeMap_iterator it = this->_chanModeMap.begin(); it !=  this->_chanModeMap.end();)
	{
		currentIt = it;
		++it;
		this->_unloadChanMode(currentIt);
	}
}

bool	Server::_unloadChanMode(Server::chanModeMap_iterator it)
{
	if (it != this->_chanModeMap.end())
	{
		delete it->second;
		this->_chanModeMap.erase(it);
		return true;
	}
	return false;
}

void	Server::_loadUserModes(void)
{
	this->_loadUserMode(new OperUserMode());
	this->_loadUserMode(new InvisibleUserMode());
}

void	Server::_loadUserMode(AUserMode* newUserMode)
{
	this->_userModeMap.insert(std::make_pair(newUserMode->getMode(), newUserMode));
}


bool	Server::_unloadChanMode(char modeName)
{
	return this->_unloadChanMode(this->_chanModeMap.find(modeName));
}

AChanMode*	Server::chanModeFind(char modeName)
{
	chanModeMap_iterator	it;

	it = this->_chanModeMap.find(modeName);
	if (it == this->_chanModeMap.end())
		return NULL;
	return it->second;
}

AUserMode*	Server::userModeFind(char modeName)
{
	userModeMap_iterator	it;

	it = this->_userModeMap.find(modeName);
	if (it == this->_userModeMap.end())
		return NULL;
	return it->second;
}

Server&	Server::getInstance(void)
{
	return *Server::_instance;
}
Server*	Server::createInstance(const std::string& listenIp, int listenPort, const std::string& name, const std::string& password, const std::string& realName)
{
	if (Server::_instance == NULL)
		Server::_instance = new Server(listenIp, listenPort, name, password, realName);
	return Server::_instance;
}

void	Server::deleteInstance(void)
{
	delete Server::_instance;
	Server::_instance = NULL;
}

void	Server::_setSignals(void)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, Server::signalHandler);
}

Server::userMap_type&	Server::getUserMap(void)
{
	return this->_userMap;
}

Server::channelMap_type&	Server::getChannelMap(void)
{
	return this->_channelMap;
}

int		Server::count(void)
{
	return this->_fdMap.size();
}

const int&	Server::getFd(void) const
{
	return this->_fd;
}

void	Server::setPass(const std::string& value)
{
	this->_pass = value;
}

const std::string&	Server::getPass(void) const
{
	return this->_pass;
}

void	Server::setPollout(User& user)
{
	this->_pollfds[user.getPollIndex()].events |= POLLOUT;
}

ssize_t	Server::send(void)
{
	return this->send("");
}

ssize_t	Server::send(const std::string& data)
{
	Server::fdMap_iterator	it;

	for (it = this->_fdMap.begin(); it != this->_fdMap.end(); it++)
		it->second->send(data);
	return 0;
}

ssize_t	Server::send(const Message& message)
{
	return this->send(message.toString());
}

void	Server::setStatus(int value)
{
	this->_status = value;
}

int		Server::getStatus(void)
{
	return this->_status;
}
void	Server::setSenderStatus(ISender& sender, int value)
{
	sender.setStatus(value);
	if (value == LEVEL_REGISTERED)
		this->_eventHandler.raise(REGUSEREVENT, this->_message);
}

void	Server::quit(const std::string& msg)
{
	Server::fdMap_iterator	currentIt;

	for (Server::fdMap_iterator it = this->_fdMap.begin(); it != this->_fdMap.end();)
	{
		currentIt = it;
		it++;
		this->deleteUser(*currentIt->second, msg);
	}
	this->_stop = true;
}

void	Server::run(void)
{
	this->_listen();
	this->_loop();
}

void	Server::_initSocket(void)
{
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (!this->_fd)
	{
		Console::log(LOG_ERROR, "Server::initSocket function socket() failed");
		exit(EXIT_FAILURE);
	}
	if (fcntl(this->_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		Console::log(LOG_ERROR, "Server::initSocket function fcntl() failed");
		exit(EXIT_FAILURE);
	}
	this->_opt = 1;
	this->_addrlen = sizeof(this->_address);
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &this->_opt, sizeof(this->_opt)))
	{
		Console::log(LOG_ERROR, "Server::initSocket function setsockopt() failed");
		exit(EXIT_FAILURE);
	}
}

int	Server::_freePollIndexFind(void)
{
	for (int i = 2; i < MAXUSERS + 2; i++)
	{
		if (this->_pollfds[i].fd == 0)
			return i;
	}
	return 0;
}

void	Server::setReal(const std::string& value)
{
	this->_real = value;
}

const std::string&	Server::getReal(void) const
{
	return this->_real;
}

const std::string&	Server::getName(void) const
{
	return this->_name;
}

const std::string&	Server::getMask(void) const
{
	return this->_name;
}

bool	Server::isUser(void)
{
	return this->_type == TYPE_USER;
}

bool	Server::isServer(void)
{
	return this->_type == TYPE_SERVER;
}

int	Server::getType(void)
{
	return this->_type;
}
void	Server::setIdleTime(time_t value)
{
	this->_idleTime = value;
}

User*	Server::_accept(void)
{
	User*				user;
	int					newFd;
	struct sockaddr_in	user_addr;
	int					user_addrlen;

	user_addrlen = sizeof(struct sockaddr_in);
	newFd = accept(this->_fd, reinterpret_cast<struct sockaddr* >(&user_addr), reinterpret_cast<socklen_t* >(&user_addrlen));
	if (newFd < 0)
	{
		Console::log(LOG_ERROR, "Server::accept function accept() failed");
		exit(EXIT_FAILURE);
	}
	if (this->_fdMap.size() == MAXUSERS)
	{
		::send(newFd, "The server is full. Please, try again more later.", 49, 0);
		close(newFd);
		return NULL;
	}
	user = new User(newFd, *this);
	user->setHost(inet_ntoa(user_addr.sin_addr));
	user->setPollIndex(this->_freePollIndexFind());
	this->_pollfds[user->getPollIndex()].fd = newFd;
	this->_pollfds[user->getPollIndex()].events = POLLIN;
	return user;
}

void	Server::_bind(void)
{
	std::ostringstream	ss;

	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(this->_port);
	if (bind(this->_fd, reinterpret_cast<struct sockaddr *>(&this->_address), sizeof(this->_address)) < 0)
	{
		ss << "Server::bind function bind() failed: " << std::strerror(errno);
		Console::log(LOG_ERROR, ss.str());
		ss.clear();
		exit(EXIT_FAILURE);
	}
	ss << "IP listen: " << this->_ip << ":" << this->_port;
	Console::log(LOG_INFO, ss.str());
}

void	Server::_listen(void)
{
	if (listen(this->_fd, MAXLISTEN) < 0)
	{
		Console::log(LOG_ERROR, "Server::listen function listen() failed");
		exit(EXIT_FAILURE);
	}
	this->_pollfds[0].fd = this->_fd;
	this->_pollfds[0].events = POLLIN;
	this->_pollfds[1].fd = 0;
	this->_pollfds[1].events = POLLIN;
}

int	Server::_poll(void)
{
	return poll(this->_pollfds, MAXUSERS + 2, this->_pollTimeout);
}

void	Server::eraseChannel(Channel& channel)
{
	this->_channelMap.erase(strToUpper(channel.getName()));
	delete &channel;
}

Channel*	Server::insertChannel(const std::string& name, const User& user)
{
	Channel	*channel = new Channel(name, user, *this);

	this->_channelMap.insert(std::make_pair(strToUpper(name), channel));
	return channel;
}

void	Server::insertUser(User* user)
{
	this->_message.setReceiver(user);
	this->_fdMap.insert(std::make_pair(user->getFd(), user));
	this->_eventHandler.raise(NEWUSEREVENT, this->_message);
}

void	Server::eraseUser(User& user)
{
	this->_fdMap.erase(user.getFd());
}

bool	Server::checkChannelMode(Message& message, int commandEvent)
{
	AChanMode::Access	access = AChanMode::def;
	int					numeric = 0;

	if (static_cast<User*>(message.getSender())->isOper())
		return true;
	for (chanModeMap_iterator it = this->_chanModeMap.begin();
			it != this->_chanModeMap.end() && access != AChanMode::allow;
			++it)
	{
		if (it->second->getConfig().events & commandEvent)
			it->second->onChanEvent(access, commandEvent, message, numeric);
	}
	if (access == AChanMode::deny)
		message.replyNumeric(numeric);
	else
		Numeric::clear();
	return access != AChanMode::deny;
}

void	Server::removeUserFromChannel(Channel& channel, User& user)
{
	user.erase(&channel);	// se elimina al canal del usuario
	channel.erase(&user);	// se elimina al usuario del canal
	if (channel.empty())	// si no quedan usuarios en el canal
	{
		this->_eventHandler.raise(DELCHANEVENT, this->_message);
		this->_invite.erase(&channel);
		this->eraseChannel(channel);	// se elimina
	}
	else
	{
		channel.mode.erase('o', &user);
		channel.mode.erase('v', &user);
	}
}

void	Server::deleteUser(User& user, std::string text)
{
	Server::channelMap_iterator	currentIt;
	Server::userVector_type*	userVector = user.getUniqueVector();

	this->_message.clear();
	this->_message.setSender(&user);
	this->_message.setReceiver(&user);
	this->_message.insertField(text);
	this->_eventHandler.raise(DELUSEREVENT, this->_message);
	if (user.getStatus() == LEVEL_REGISTERED) //Si está registrado se verifica que esté en más canales
	{
		this->_message.setCmd("QUIT");
		this->_message.clearReceiver();
		this->_message.setReceiver(*userVector);
		this->_message.hideReceiver();
		this->_eventHandler.raise(QUITEVENT, this->_message);
		for (Server::channelMap_iterator it = user.begin(); it != user.end();)
		{
			currentIt = it;
			++it;
			this->_message.setChannel(currentIt->second);
			this->removeUserFromChannel(*currentIt->second, user);
		}
	}
	if (!user.getName().empty())
		this->_userMap.erase(strToUpper(user.getName()));
	this->_fdMap.erase(user.getFd());
	this->_pollfds[user.getPollIndex()].fd = 0;
	this->_pollfds[user.getPollIndex()].events = 0;
	this->_message.clear();
	this->_invite.erase(&user);
	delete &user;
	delete userVector;
}

void	Server::addToChannel(Message& message)
{
	Channel*					channel = NULL;
	const std::string&			channelName = message[0];
	User*						user = static_cast<User*>(message.getSender());
	Server::userMap_insert		retUser;

	if (this->isChannel(channelName))
	{
		if (!isValidChanName(channelName))
		{
			Numeric::insertField(channelName);
			message.replyNumeric(ERR_BADCHANMASK);
			return ;
		}
		if (user->size() == MAXCHANNEL)
			this->_eventHandler.raise(MAXCHANEVENT, this->_message);
		// el canal no existe, se ha de crear
		else if (!(channel = this->channelAt(channelName)))
		{
			channel = insertChannel(channelName, *user);
			// añade el canal al usuario y el usuario al canal
			channel->insert(user);
			user->insert(channel);
			channel->mode.insert('o', user);
			message.setChannel(channel);
			this->_eventHandler.raise(NEWCHANEVENT, this->_message);
		}
		else
		{
			message.setChannel(channel);
			if (user->isOnChannel(*channel))
				this->_eventHandler.raise(ALREADYEVENT, this->_message);
			else
			{
				if (!this->checkChannelMode(message, COMMAND_JOIN))
					return ;
				channel->insert(user);
				//eliminamos la invitación, si existiera.
				if (this->_invite.erase(user, channel))
					Console::log(LOG_INFO, "User <" + user->getName() + "> has accepted its invitation");
				// añade el canal al usuario y el usuario al canal
				channel->insert(user);
				user->insert(channel);
				this->_eventHandler.raise(JOINEVENT, this->_message);
			}
		}
	}
	else
		this->_eventHandler.raise(ERRCHANEVENT, this->_message);
}

void	Server::delFromChannel(Message& message)
{
	Channel*					channel = NULL;
	User*						user = static_cast<User*>(message.getSender());
	const std::string&			channelName = message[0];
	Server::channelMap_iterator	it;
	
	if (this->isChannel(channelName))
	{
		if ((channel = this->channelAt(channelName)))
		{
			message.setChannel(channel);
			if ((it = user->find(channelName)) == user->end())
				this->_eventHandler.raise(NOTINCHANEVENT, this->_message);
			else
			{
				this->_eventHandler.raise(PARTEVENT, this->_message);
				this->removeUserFromChannel(*channel, *user);
			}
		}
		else
			this->_eventHandler.raise(NOTCHANEVENT, this->_message);
	}
	else
		this->_eventHandler.raise(ERRCHANEVENT, this->_message);
}

int	Server::_checkUserConnection(void)
{
	User*	user;

	if (this->_pollfds[0].revents & POLLIN)
	{
		user = this->_accept();
		if (!user)
		{
			Console::log(LOG_WARNING, "Server full, rejecting new connection: MAXUSERS = " + to_string(MAXUSERS));
			return 1;
		}
		this->insertUser(user);
		Console::log(LOG_INFO, "User <anonymous> connected");
		return 1;
	}
	return 0;
}

void	Server::chanModeNames(Channel& channel)
{
	const ChanModeConfig*				modeConfig;
	Channel::Mode::multimap_iterator	modeIt;
	std::string							modeStr("+");
	std::deque<std::string>				strDeque;

	for (Server::chanModeMap_iterator it = this->_chanModeMap.begin()
			; it != this->_chanModeMap.end()
			; ++it)
	{
		modeConfig = &it->second->getConfig();
		if (modeConfig->unique
				&& (modeIt = channel.mode.find(modeConfig->mode)) != channel.mode.end())
		{
			modeStr += modeConfig->mode;
			if (modeConfig->type & ChanModeConfig::enableParam)
				strDeque.push_front(it->second->toString(modeIt->second));
		}
	}
	Numeric::insertField(modeStr);
	for (std::deque<std::string>::iterator it = strDeque.begin()
			; it != strDeque.end()
			; ++it)
		Numeric::insertField(*it);
}

void	Server::userModeNames(User& user)
{
	AUserMode*		userMode;
	std::string		modeStr("+");

	for (Server::userModeMap_iterator it = this->_userModeMap.begin()
			; it != this->_userModeMap.end()
			; ++it)
	{
		userMode = it->second;
		if (user.isSetMode(userMode))
		{
			modeStr += userMode->getMode();
		}
	}
	Numeric::insertField(modeStr);
}

void	Server::supportNames(void)
{
	for (supportMap_iterator it = this->_supportMap.begin()
			; it != this->_supportMap.end()
			; ++it)
		Numeric::insertField(it->first + "=" + it->second);
}

void	Server::names(Channel& channel)
{
	std::string	mode;

	//TODO hay que determinar si el canal tiene +p, se enviará "*", +s "@" o nada "="
	Numeric::insertField("=");
	Numeric::insertField(channel.getName());
	for (Server::userMap_iterator it = channel.begin(); it != channel.end(); it++)
	{
		if (channel.isOper(it->second))
			mode += "@";
		if (channel.isVoice(it->second))
			mode += "+";
		Numeric::insertField(mode + it->second->getName());
		mode.clear();
	}
}

void	Server::_checkUserIO(void)
{
	User*		user;
	std::size_t	size;

	for (int i = 2; i < MAXUSERS + 2; i++)
	{
		if (this->_pollfds[i].revents & POLLIN)
		{
			user = this->_fdMap[this->_pollfds[i].fd];
			size = user->checkInput(this->_pollfds[i].fd, this->_message);
			if (size <= 0) // ctrl+c
				this->deleteUser(*user, "Client exited");
		}
		else if (this->_pollfds[i].revents & POLLOUT)
		{
			user = this->_fdMap[this->_pollfds[i].fd];
			if (user->checkOutput(this->_pollfds[i].fd))
				this->_pollfds[i].events ^= POLLOUT;
		}
		else if (this->_pollfds[i].fd > 0)
		{
			user = this->_fdMap[this->_pollfds[i].fd];
			this->_checkUserTimeout(*user);
		}
	}
}

bool	Server::recvCommand(Message& message)
{
	ACommand*	command;

	static_cast<User*>(message.getSender())->setIdleInner(time(NULL));
	if ((command = this->commandFind(message.getCmd())))
	{
		command->recv(message);
		return true;
	}
	return false;
}

bool	Server::sendCommand(Message& message)
{
	ACommand*	command;

	if ((command = this->commandFind(message.getCmd())))
	{
		if (message.getReceiver() == NULL)
			message.setReceiver(message.getSender());
		command->send(message);
		return true;
	}
	return false;
}
void	Server::_checkConsoleInput(void)
{
	std::string		buffer;

	if (this->_pollfds[1].revents & POLLIN)
	{
		std::getline(std::cin, buffer);
		if (std::cin && buffer == "quit")
			this->quit(SHUTDOWN_STRING);
	}
}

void	Server::_checkUserTimeout(User& user)
{
	if (user.getNextTimeout() && user.getNextTimeout() < std::time(NULL))
		this->deleteUser(user, "Registration timeout");
	else if (!user.getNextTimeout() && (user.getIdleInner() + IDLETIMEOUT < std::time(NULL)))
	{
		if (user.getStatus() == LEVEL_REGISTERED)
		{
			user.setNextTimeout(time(NULL) + NEXTTIMEOUT);
			user.setPingChallenge(this->_name);
		}
		user.send("PING :" + user.getPingChallenge());
	}
}

void	Server::_checkTimeout(void)
{
	Server::fdMap_iterator	it;

	for (it = this->_fdMap.begin(); it != this->_fdMap.end();)
		this->_checkUserTimeout(*(it++)->second);
}

void	Server::_loop(void)
{
	int	rv;

	Console::log(LOG_INFO, "Waiting to connect clients...");
	while (!this->_stop)
	{
		rv = this->_poll();
		if (rv == -1)
		{
			if (this->_stop)
			{
				Console::log(LOG_ERROR, "Server stopped by user");
				exit(0);
			}
			Console::log(LOG_ERROR, "Server::loop function poll() failed");
			exit(EXIT_FAILURE);
		}
		else if (rv == 0)
			this->_checkTimeout();
		else
		{
			if (!this->_checkUserConnection())
				this->_checkUserIO();
			this->_checkConsoleInput();
		}
	}
}
