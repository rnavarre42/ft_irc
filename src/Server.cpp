#include "Server.hpp"
#include "User.hpp"
#include <cerrno>
#include "Channel.hpp"
#include "Console.hpp"
#include "Message.hpp"
#include "chanmodes.hpp"
#include "commands.hpp"
#include "Numeric.hpp"
#include "ChanModeConfig.hpp"

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

//	socket, setsockopt, getsockname, getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo, bind, connect
//	listen, accept, htons, htonl, ntohs, ntohl, inet_addr, inet_htoa, send, recv, signal, lseek, fstat, fcntl
//	poll

//template <typename T>
//class EventHandler;

Server	*Server::_instance = NULL;

void	Server::signalHandler(int sig)
{
	Server &server = Server::getInstance();

	std::cout << std::endl;
	if (sig == SIGINT)
		server.quit(SHUTDOWN_STRING);
}

const char	*Server::ServerFullException::what(void) const throw()
{
	return "The server is full.";
}

Server::Server(std::string listenIp, int listenPort, std::string name)
	: _ip(listenIp)
	, _port(listenPort)
	, _message(*new Message(*this))
	, _name(name)
	, _type(TYPE_SERVER)
{
	this->_loadCommands();
	this->_loadChanModes();
//	this->_logger();
	this->_setSignals();
	std::memset(this->_pollfds, '\0', sizeof(struct pollfd) * (MAXUSERS + 2));
	this->_stop = false;
	this->_initSocket();
	this->_pollTimeout = 1000;
	this->_status = 0;
	this->_bind();
//	this->_source.server = this;
}

Server::~Server(void)
{
	this->_unloadCommands();
	this->_unloadChanModes();
	delete &this->_message;
	Console::log(LOG_INFO, "Server destroyed");
}

void	Server::_loadCommands(void)
{

//	this->_commandMap["AWAY"]		= new AwayCommand	(*this, LEVEL_REGISTERED, 0);
	this->_commandMap["INVITE"]		= new InviteCommand	(*this, LEVEL_REGISTERED, 2);
	this->_commandMap["JOIN"]		= new JoinCommand	(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["KICK"]		= new KickCommand	(*this, LEVEL_REGISTERED, 2);
	this->_commandMap["MODE"]		= new ModeCommand	(*this, LEVEL_REGISTERED, 1);
	this->_commandMap["MOTD"]		= new MotdCommand	(*this, LEVEL_REGISTERED, 0);
	this->_commandMap["NICK"]		= new NickCommand	(*this, LEVEL_ALL, 1);
	this->_commandMap["NOTICE"]		= new NoticeCommand	(*this, LEVEL_REGISTERED, 2);
	this->_commandMap["PART"]		= new PartCommand	(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["PASS"]		= new PassCommand	(*this, LEVEL_UNREGISTERED, 1);
	this->_commandMap["PING"]		= new PingCommand	(*this, LEVEL_REGISTERED, 1);
	this->_commandMap["PONG"]		= new PongCommand	(*this, LEVEL_NEGOTIATING | LEVEL_REGISTERED, 1);
	this->_commandMap["PRIVMSG"]	= new PrivmsgCommand(*this, LEVEL_REGISTERED, 1);
	this->_commandMap["QUIT"]		= new QuitCommand	(*this, LEVEL_ALL, 0);
	this->_commandMap["SHUTDOWN"]	= new ShutdownCommand	(*this, LEVEL_IRCOPERATOR, 0);
	this->_commandMap["TOPIC"]		= new TopicCommand	(*this, LEVEL_REGISTERED, 1);
	this->_commandMap["USER"]		= new UserCommand	(*this, LEVEL_UNREGISTERED, 4);

//	this->_commandMap["WHO"]		= new WhoCommand	(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["KILL"]		= new KillCommand	(*this, LEVEL_IRCOPERATOR, 2);
//	this->_commandMap["LIST"]		= new ListCommand	(*this, LEVEL_REGISTERED, 0);
//	this->_commandMap["WHOIS"]		= new WhoisCommand	(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["WHOWAS"]		= new WhowasCommand	(*this, LEVEL_REGISTERED, 1);
	this->_commandMap["NAMES"]		= new NamesCommand	(*this, LEVEL_REGISTERED, 1);

	Server::aCommandMap_iterator	it;

	for (it = this->_commandMap.begin(); it != this->_commandMap.end(); it++)
		it->second->loadEvents(this->_eventHandler);
}

void	Server::_unloadCommands(void)
{
	for (Server::aCommandMap_iterator it = this->_commandMap.begin(); it != this->_commandMap.end(); it++)
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

void	Server::_unloadChanModes(void)
{
	Server::aChanModeMap_iterator currentIt;

	for (Server::aChanModeMap_iterator it = this->_chanModeMap.begin(); it !=  this->_chanModeMap.end();)
	{
		currentIt = it;
		it++;
		this->_unloadChanMode(currentIt);
	}
}

void	Server::_loadChanMode(AChanMode *newChanMode)
{
	ChanModeConfig const &chanModeConfig = newChanMode->getConfig();
	this->_chanModeMap[chanModeConfig.mode] = newChanMode;
}

bool	Server::_unloadChanMode(Server::aChanModeMap_iterator it)
{
	if (it != this->_chanModeMap.end())
	{
		delete it->second;
		this->_chanModeMap.erase(it);
		return true;
	}
	return false;
}

bool	Server::_unloadChanMode(char modeName)
{
	return this->_unloadChanMode(this->_chanModeMap.find(modeName));
}

AChanMode	*Server::findChanMode(char modeName)
{
	aChanModeMap_iterator	it;

	it = this->_chanModeMap.find(modeName);
	if (it == this->_chanModeMap.end())
		return NULL;
	return it->second;
}

Server	&Server::getInstance(void)
{
	return *Server::_instance;
}
Server	&Server::createInstance(std::string listenIp, int listenPort, std::string name)
{
	if (Server::_instance == NULL)
		Server::_instance = new Server(listenIp, listenPort, name);
	return *Server::_instance;
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

Server::userMap_type	&Server::getUserMap(void)
{
	return this->_userMap;
}

Server::channelMap_type	&Server::getChannelMap(void)
{
	return this->_channelMap;
}

int		Server::count(void)
{
	return this->_fdMap.size();
}

int	const	&Server::getFd(void) const
{
	return this->_fd;
}

void	Server::setPass(const std::string& value)
{
	this->_pass = value;
}

std::string	const	&Server::getPass(void) const
{
	return this->_pass;
}

void	Server::setPollout(User &user)
{
	this->_pollfds[user.getPollIndex()].events |= POLLOUT;
}

ssize_t	Server::send(const std::string& msg)
{
	//int usersLeft;
	Server::fdMap_iterator	it;

	//usersLeft = this->fdMap.size();
	for (it = this->_fdMap.begin(); it != this->_fdMap.end(); it++)
		it->second->send(msg);
/*
	for (size_t i = 1; usersLeft; i++)
	{
		if (this->pollfds[i].fd)
		{
			this->fdMap[this->pollfds[i].fd]->send(msg);
			usersLeft--;
		}
	}
*/
//	for (std::vector<User *>::iterator it = userVector.begin(); it != userVector.end(); it++)
//		it->sendTo(msg);
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
void	Server::setSenderStatus(ISender &sender, int value)
{
	sender.setStatus(value);
	if (value == LEVEL_REGISTERED)
		this->_eventHandler.raise(REGUSEREVENT, this->_message);
}

void	Server::quit(std::string msg)
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

inline int Server::_freePollIndexFind(void)
{
	for (int i = 2; i < MAXUSERS + 2; i++)
	{
		if (this->_pollfds[i].fd == 0)
			return i;
	}
	return 0;
}

std::string const	&Server::getName(void) const
{
	return this->_name;
}

std::string			Server::getMask(void)
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

bool	Server::isOper(void)
{
	return false;
}

int	Server::getType(void)
{
	return this->_type;
}
void	Server::setIdleTime(time_t value)
{
	this->_idleTime = value;
}

User	*Server::_accept(void)
{
	User				*user;
	int					newFd;
	struct sockaddr_in	user_addr;
	int					user_addrlen;

	user_addrlen = sizeof(struct sockaddr_in);
	newFd = accept(this->_fd, reinterpret_cast<struct sockaddr *>(&user_addr), reinterpret_cast<socklen_t *>(&user_addrlen));
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

int		Server::_poll(void)
{
	return poll(this->_pollfds, MAXUSERS + 2, this->_pollTimeout);
}

void	Server::delChannel(Channel &channel)
{
	this->_channelMap.erase(strToUpper(channel.getName()));
	delete &channel;
}

void	Server::createUser(User &user)
{
	this->_message.setReceiver(&user);
	this->_fdMap[user.getFd()] = &user;
	this->_eventHandler.raise(NEWUSEREVENT, this->_message);
}

void	Server::_removeUserFromChannel(Channel &channel, User &user)
{
	user.erase(&channel);	// se elimina al canal del usuario
	channel.erase(&user);	// se elimina al usuario del canal
	if (channel.empty())	// si no quedan usuarios en el canal
	{
//		this->_source.message->setChannel(&channel);
		this->_eventHandler.raise(DELCHANEVENT, this->_message);
		this->_invite.erase(&channel);
		this->delChannel(channel);	// se elimina
	}
}

/*
void	displayUserName(User *user)
{
	std::cout << "User name = " << user->getName() << std::endl;
}
*/
/*
Channel	*findFullestChannel(User &user)
{
	Server::channelMap_iterator	currentIt = user.getChannelMap().begin();
	Server::channelMap_iterator	nextIt = currentIt;

	for (; nextIt != user.getChannelMap().end(); nextIt++)
	{
		if (nextIt->second->getUserMap().size() > currentIt->second->getUserMap().size())
			currentIt = nextIt;
	}
	return	currentIt->second;
}

Server::userVector_type	*getUserVector(User &user)
{
	//aloca memoria para el nuevo vector
	Server::userVector_type								*userVector;
	Server::channelSet_type								channelSet;
	Channel												*currentChannel;
	std::pair<Server::channelSet_iterator, bool>		ret;
	Server::userVector_iterator							vectorIt;

	if (!user.getChannelMap().size())
		return NULL;
	//buscar el canal que tiene mas usuarios
	currentChannel = findFullestChannel(user);
	//std::cout << "channel name = " << currentChannel->getName() << std::endl;

	//añadir los usuarios del canal mas grande al vector
	channelSet.insert(currentChannel);
	userVector = new Server::userVector_type;
	for (Server::userPairMap_iterator it = currentChannel->getUserMap().begin(); it != currentChannel->getUserMap().end(); it++)
		userVector->push_back(it->second.second);
	//añadir los usuarios de los canales restantes sin repetir usuario en el vector
	for (Server::channelMap_iterator it = user.getChannelMap().begin(); it != user.getChannelMap().end(); it++)
	{
		currentChannel = it->second;
		ret = channelSet.insert(currentChannel);
		if (ret.second == true)
			for (Server::userPairMap_iterator it = currentChannel->getUserMap().begin(); it != currentChannel->getUserMap().end(); it++)
			{
				vectorIt = std::find(userVector->begin(), userVector->end(), it->second.second);
				if (vectorIt == userVector->end())
					userVector->push_back(it->second.second);
			}
	}
//	for_each(userVector->begin(), userVector->end(), displayUserName);
	return userVector;
}
*/
void	Server::deleteUser(User &user, std::string text)
{
	Server::channelMap_iterator	currentIt;
	Server::userVector_type*	userVector = user.getUniqueVector();

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
			it++;
			this->_message.setChannel(currentIt->second);
			this->_removeUserFromChannel(*currentIt->second, user);
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

// :masksource JOIN #CHAN :Pass
void	Server::addToChannel(Message &message)
{
	Channel											*channel = NULL;
	std::string										&channelName = message[0];
	User											&user = static_cast<User &>(*message.getSender());
	Server::channelMap_iterator						it;
	std::pair<Server::userMap_iterator, bool>		retUser;

//	this->_source.message = &message;
	if (this->isChannel(channelName))
	{
//		std::cout << "user " << user.getName() << " " << user.getChannelMap().size() << std::end;
		if (user.size() == MAXCHANNEL)
			this->_eventHandler.raise(MAXCHANEVENT, this->_message);
		// el canal no existe, se ha de crear
		else if ((it = this->channelFind(channelName)) == this->_channelMap.end())
		{
			channel = new Channel(channelName, user, *this);
			this->_channelMap[strToUpper(channelName)] = channel;
			// añade el canal al usuario y el usuario al canal
			channel->insert(&user);
			user.insert(channel);
			//user[strToUpper(channelName)] = channel;
			//TODO falta dar op al primero que entra.
			channel->mode.insert('o', &user);
			message.setChannel(channel);
			this->_eventHandler.raise(NEWCHANEVENT, this->_message);
			//this->_eventHandler.raise(JOINEVENT, this->_source);
		}
		else
		{

			//TODO hay que limpiar este código y tratar de hacerlo más compacto
			channel = it->second;
			message.setChannel(channel);
		
			//eliminamos la invitación, si existiera.
			if (this->_invite.erase(&user, channel))
			{
				Console::log(LOG_INFO, user.getName() + " ha hecho efectiva su invitacion");
			}
			else
			{
				Console::log(LOG_INFO, user.getName() + " no tenia invitacion");
			}

			retUser = channel->insert(&user);
			
			if (retUser.second == true)  //El nick no está en el canal
			{
				// añade el canal al usuario y el usuario al canal
				channel->insert(&user);
				user.insert(channel);
				this->_eventHandler.raise(JOINEVENT, this->_message);
			}
			else
				this->_eventHandler.raise(ALREADYEVENT, this->_message);
			//TODO falta por gestionar +l +i +k
		}
	}
	else
		this->_eventHandler.raise(ERRCHANEVENT, this->_message);
}

void	Server::delFromChannel(Message &message)
{
	Channel							*channel = NULL;
	User							&user = static_cast<User &>(*message.getSender());
	std::string						&channelName = message[0];
	Server::channelMap_iterator		it;	
	
	if (this->isChannel(channelName))
	{
		if ((it = this->channelFind(channelName)) == this->_channelMap.end())
			this->_eventHandler.raise(NOTCHANEVENT, this->_message);
		else
		{
			channel = it->second;
			message.setChannel(channel);
			if ((it = user.find(channelName)) == user.end())
				this->_eventHandler.raise(NOTINCHANEVENT, this->_message);
			else
			{
				this->_eventHandler.raise(PARTEVENT, this->_message);
				this->_removeUserFromChannel(*channel, user);
			}
		}
	}
	else
		this->_eventHandler.raise(ERRCHANEVENT, this->_message);
}

int	Server::_checkUserConnection(void)
{
	User	*user;

	if (this->_pollfds[0].revents & POLLIN)
	{
		user = this->_accept();
		if (!user)
		{
			Console::log(LOG_WARNING, "Server full, rejecting new connection");
			return 1;
		}
/*
 		if (user->send("Hello\r\n") <= 0)
		{
			Console::log(LOG_ERROR, "Server::checkUserConnection function user->sendTo() failed");
			exit(EXIT_FAILURE);
		}
*/
		this->createUser(*user);
		Console::log(LOG_INFO, "User <anonymous> connected");
		return 1;
	}
	return 0;
/*
 * Excepción eliminada al devolver _accept un puntero y no una referencia
 *
 *		catch (Server::ServerFullException &e)
 *		{
 *			std::cerr << e.what() << std::endl;
 *		}
 */
}

void	Server::names(Channel &channel)
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
	User	*user;
	size_t	size;

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
		//	std::cout << "El usuario " << user->getName() << " ya acepta mensajes" << std::endl;
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

bool	Server::recvCommand(Message &message)
{
	ACommand	*command;

	message.getSender()->setIdleTime(time(NULL));
	if ((command = this->commandFind(message.getCmd())))
	{
		command->recv(message);
		return true;
	}
	return false;
}

bool	Server::sendCommand(Message &message)
{
	ACommand	*command;

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
	int		size;
	char	buffer[BUFFERSIZE + 1];
	std::string	str = "console> ";

	if (this->_pollfds[1].revents & POLLIN)
	{
		size = read(0, buffer, BUFFERSIZE);
		buffer[size] = '\0';
		if (size > 0)
		{
			if (!strcmp(buffer, "quit\n"))
				this->quit(SHUTDOWN_STRING);
			if (!strcmp(buffer, "invite\n"))
			{
				std::cout << "Nick     Channel" << std::endl;
				for (Invite::inviteVector_iterator it = _invite.begin(); it != _invite.end(); it++)
				{
					std::cout << it->first->getName() << "\t" << it->second->getName() << std::endl;
				}
			}
		}
	}
}

void	Server::_checkUserTimeout(User &user)
{
	if (user.getNextTimeout() && user.getNextTimeout() < time(NULL))
		this->deleteUser(user, "Registration timeout");
	else if (!user.getNextTimeout() && (user.getIdleTime() + IDLETIMEOUT < time(NULL)))
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

	Console::log(LOG_INFO, "Waiting connect clients...");
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
