#include "Server.hpp"
#include "Channel.hpp"
#include "Console.hpp"
#include "Message.hpp"
#include "commands.hpp"

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


//	socket, setsockopt, getsockname, getprotobyname, gethostbyname, getaddrinfo, freeaddrinfo, bind, connect
//	listen, accept, htons, htonl, ntohs, ntohl, inet_addr, inet_htoa, send, recv, signal, lseek, fstat, fcntl
//	poll

//template <typename T>
//class EventHandler;

Server	*Server::_instance = NULL;

void	Server::signalHandler(int sig)
{
	Server &server = Server::getInstance();

	if (sig == SIGINT)
		server.quit("Shutdown. Please, reconnect to another server.\n");
}

const char	*Server::ServerFullException::what(void) const throw()
{
	return "The server is full.";
}

Server::Server(std::string listenIp, int listenPort, std::string name)
	: _ip(listenIp), _port(listenPort), _name(name), _type(TYPE_SERVER)
{
	this->_loadCommands();
//	this->_logger();
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, Server::signalHandler);
	std::memset(this->_pollfds, '\0', sizeof(struct pollfd) * (MAXUSERS + 2));
	this->_stop = false;
	this->_initSocket();
	this->_pollTimeout = 1000;
	this->_registered = 0;
	this->_bind();
	this->_source.server = this;
}

Server::~Server(void)
{
	this->_unloadCommands();
	Console::log(LOG_INFO, "Server destroyed");
}

void	Server::_loadCommands(void)
{

//	this->_commandMap["AWAY"]	= new AwayCommand	(*this, LEVEL_REGISTERED, 0);
	this->_commandMap["JOIN"]	= new JoinCommand	(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["KICK"]	= new KickCommand	(*this, LEVEL_REGISTERED, 2);
	this->_commandMap["MOTD"]	= new MotdCommand	(*this, LEVEL_REGISTERED, 0);
	this->_commandMap["NICK"]	= new NickCommand	(*this, LEVEL_ALL, 1);
	this->_commandMap["PART"]	= new PartCommand	(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["PASS"]	= new PassCommand	(*this, LEVEL_UNREGISTERED, 1);
	this->_commandMap["PING"]	= new PingCommand	(*this, LEVEL_REGISTERED, 1);
	this->_commandMap["PONG"]	= new PongCommand	(*this, LEVEL_ALL, 1);
	this->_commandMap["PRIVMSG"]	= new PrivmsgCommand(*this, LEVEL_REGISTERED, 1);
	this->_commandMap["QUIT"]	= new QuitCommand	(*this, LEVEL_ALL, 0);
	this->_commandMap["USER"]	= new UserCommand	(*this, LEVEL_UNREGISTERED, 4);

//	this->_commandMap["WHO"]		= new WhoCommand	(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["KILL"]	= new KillCommand	(*this, LEVEL_IRCOPERATOR, 2);
//	this->_commandMap["LIST"]	= new ListCommand	(*this, LEVEL_REGISTERED, 0);
//	this->_commandMap["MODE"]	= new ModeCommand	(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["WHOIS"]	= new WhoisCommand	(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["INVITE"]	= new InviteCommand	(*this, LEVEL_REGISTERED, 2);
//	this->_commandMap["NOTICE"]	= new NoticeCommand	(*this, LEVEL_REGISTERED, 2);
//	this->_commandMap["WHOWAS"]	= new WhowasCommand	(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["NAMES"]	= new NamesCommand	(*this, LEVEL_REGISTERED, 0)

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
}

Server	&Server::getInstance(void)
{
	return *Server::_instance;
}
Server	&Server::getInstance(std::string listenIp, int listenPort, std::string name)
{
	if (Server::_instance == NULL)
		Server::_instance = new Server(listenIp, listenPort, name);
	return *Server::_instance;
}

void	Server::deleteInstance(void)
{
	delete Server::_instance;
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

void	Server::setPass(std::string value)
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

ssize_t	Server::send(std::string msg)
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

ssize_t	Server::send(Message &message)
{
	return this->send(message.toString());
}

void	Server::registerUser(Message &message)
{
	message.getSender()->setRegistered(true);
	this->_source.message = &message;
	this->_eventHandler.raise(REGUSEREVENT, this->_source);
}

void	Server::quit(std::string msg)
{
	Server::fdMap_iterator	currentIt;

	for (Server::fdMap_iterator it = this->_fdMap.begin(); it != this->_fdMap.end();)
	{
		currentIt = it;
		it++;
		this->delUser(*currentIt->second, msg);
	}
	this->_stop = true;
}

void	Server::start(void)
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

inline int Server::_findFreePollIndex(void)
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
	char				ipAddress[INET_ADDRSTRLEN];
//	socklen_t			len;

	newFd = accept(this->_fd, (struct sockaddr *)&user_addr, (socklen_t *)&user_addrlen);
	if (newFd < 0)
	{
		Console::log(LOG_ERROR, "Server::accept function accept() failed");
		exit(EXIT_FAILURE);
	}
	if (this->_fdMap.size() == MAXUSERS)
	{
		::send(newFd, "The server is full. Please, try again more later.\r\n", 51, 0);
		close(newFd);
		return NULL;
//		throw Server::ServerFullException();
	}
	user = new User(newFd, *this);
//	len = sizeof(user_addr);
//	getsockname(newFd, (struct sockaddr *)&user_addr, &len);
	user->setHost(inet_ntop(AF_INET, &user_addr.sin_addr, ipAddress, INET_ADDRSTRLEN));
	user->setPollIndex(this->_findFreePollIndex());
//	std::cerr << "setPollIndex = " << user->getPollIndex() << std::endl;
	this->_pollfds[user->getPollIndex()].fd = newFd;
	this->_pollfds[user->getPollIndex()].events = POLLIN;
	return user;
}

void	Server::_bind(void)
{
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(this->_port);
	if (bind(this->_fd, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
	{
		Console::log(LOG_ERROR, "Server::bind function bind() failed");
		exit(EXIT_FAILURE);
	}
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

void	Server::addUser(User &user)
{
	Message &message = Message::builder(*this);
	message.setReceiver(&user);
	this->_source.message = &message;
//	this->userVector.push_back(&user);
	this->_fdMap[user.getFd()] = &user;
	this->_eventHandler.raise(NEWUSEREVENT, this->_source);
	delete &message;
}

void	Server::_removeUserFromChannel(Channel &channel, User &user)
{
	user.delChannel(channel.getName());	// se elimina al canal del usuario
	channel.delUser(user.getName());	// se elimina al usuario del canal
	if (channel.empty())	// si no quedan usuarios en el canal
	{
		this->_source.message->setChannel(&channel);
		this->_eventHandler.raise(DELCHANEVENT, this->_source);
		this->delChannel(channel);	// se elimina
	}
}

void	displayUserName(User *user)
{
//<<<<<<< HEAD
//	Server::channelMap_iterator currentIt;
//	Message &message = Message::builder(*this);
//=======
	std::cout << "User name = " << user->getName() << std::endl;
}

Channel	*findFullestChannel(User &user)
{
	Server::channelMap_iterator	currentIt = user.getChannelMap().begin();
	Server::channelMap_iterator	nextIt = currentIt;

	for (; nextIt != user.getChannelMap().end(); nextIt++)
	{
		if (nextIt->second.second->getUserMap().size() > currentIt->second.second->getUserMap().size())
			currentIt = nextIt;
	}
	return	currentIt->second.second;
}

Server::userVector_type	*getUserVector(User &user)
{
	//aloca memoria para el nuevo vector
	Server::userVector_type							*userVector;
	std::set<Channel *>								channelSet;
	Channel											*currentChannel;
	std::pair<std::set<Channel *>::iterator, bool>	ret;
	Server::userVector_iterator						vectorIt;

	if (!user.getChannelMap().size())
		return NULL;
	//buscar el canal que tiene mas usuarios
	currentChannel = findFullestChannel(user);
	//std::cout << "channel name = " << currentChannel->getName() << std::endl;

	//añadir los usuarios del canal mas grande al vector
	channelSet.insert(currentChannel);
	userVector = new Server::userVector_type;
	for (Server::userMap_iterator it = currentChannel->getUserMap().begin(); it != currentChannel->getUserMap().end(); it++)
		userVector->push_back(it->second);
	//añadir los usuarios de los canales restantes sin repetir usuario en el vector
	for (Server::channelMap_iterator it = user.getChannelMap().begin(); it != user.getChannelMap().end(); it++)
	{
		currentChannel = it->second.second;
		ret = channelSet.insert(currentChannel);
		if (ret.second == true)
		{
			for (Server::userMap_iterator it = currentChannel->getUserMap().begin(); it != currentChannel->getUserMap().end(); it++)
			{
				vectorIt = std::find(userVector->begin(), userVector->end(), it->second);
				if (vectorIt == userVector->end())
					userVector->push_back(it->second);
			}
		}
	}
//	for_each(userVector->begin(), userVector->end(), displayUserName);
	return userVector;
}

void	Server::delUser(User &user, std::string text)
{
	Server::channelMap_iterator	currentIt;
	Message						&message = Message::builder(user);
	Server::userVector_type		*userVector = getUserVector(user);

	message.setReceiver(&user);
	message.insertField(text);
	this->_source.message = &message;
	this->_eventHandler.raise(DELUSEREVENT, this->_source);
	// Se elimina de userMap si tiene nick
	if (user.isRegistered()) //!getName().empty())
	{
//		this->send("User <" + user.getName() + "> disconnect\n");
		//TODO hay que informar a los demas usuarios que se ha ido (QUIT), falta el listado de los usuarios por
		//canal sin repetir.
		message.setCmd("QUIT");
		message.clearReceiver();
		if (userVector)
			message.setReceiver(*userVector);
//		else  // y en este trozo se duplicaba el usuario a eliminar.
//			message.setReceiver(&user);
		message.setBroadcast(true);
		this->_eventHandler.raise(QUITEVENT, this->_source);
		for (Server::channelMap_iterator it = user.getChannelMap().begin(); it != user.getChannelMap().end();)
		{
			currentIt = it;
			it++;
			this->_removeUserFromChannel(*currentIt->second.second, user);
		}
		this->_userMap.erase(strToUpper(user.getName()));
	}
	this->_fdMap.erase(user.getFd());
	this->_pollfds[user.getPollIndex()].fd = 0;
	this->_pollfds[user.getPollIndex()].events = 0;
//	this->userVector.erase(std::remove(this->userVector.begin(), this->userVector.end(), &user), this->userVector.end());
	delete &user;
	delete &message;
	delete userVector;
}

Server::userMap_iterator	Server::findUser(std::string &userName)
{
	return this->_userMap.find(strToUpper(userName));
}

Server::channelMap_iterator	Server::findChannel(std::string &channelName)
{
	return this->_channelMap.find(strToUpper(channelName));
}

// :masksource JOIN #CHAN :Pass
void	Server::addToChannel(Message &message)
{
	Channel											*channel = NULL;
	std::string										&channelName = message[0];
	User											&user = *static_cast<User *>(message.getSender());
	Server::channelMap_iterator						it;
	std::pair<Server::userMap_iterator, bool>		retUser;

	this->_source.message = &message;
	if (this->validChannelPrefix(channelName))
	{
//		std::cout << "user " << user.getName() << " " << user.getChannelMap().size() << std::end;
		if (user.getChannelMap().size() == MAXCHANNEL)
			this->_eventHandler.raise(MAXCHANEVENT, this->_source);
		else if ((it = this->channelFind(channelName)) == this->_channelMap.end())
		{
			channel = new Channel(channelName, user);
			this->_channelMap[strToUpper(channelName)].second = channel;
			// añade el canal al usuario y el usuario al canal
			channel->getUserMap()[strToUpper(user.getName())] = &user;
			user.getChannelMap()[strToUpper(channelName)].second = channel;
			//
			message.setChannel(channel);
			this->_eventHandler.raise(NEWCHANEVENT, this->_source);
			//this->_eventHandler.raise(JOINEVENT, this->_source);
		}
		else
		{
			channel = it->second.second;
			message.setChannel(channel);
			retUser = channel->getUserMap().insert(std::pair<std::string, User *>(strToUpper(user.getName()), &user));
			if (retUser.second == true)  //El nick ha entrado al canal
			{
				// añade el canal al usuario y el usuario al canal
				channel->getUserMap()[strToUpper(user.getName())] = &user;
				user.getChannelMap()[strToUpper(channelName)].second = channel;
				//
				this->_eventHandler.raise(JOINEVENT, this->_source);
			}
			else
				this->_eventHandler.raise(ALREADYEVENT, this->_source);
			//TODO falta por gestionar +l +i +k
		}
	}
	else
		this->_eventHandler.raise(ERRCHANEVENT, this->_source);
}

inline bool	Server::validChannelPrefix(std::string &channelName)
{
	return channelName[0] == '#';
}

void	Server::delFromChannel(Message &message)
{
	Channel							*channel = NULL;
	User							&user = *static_cast<User* >(message.getSender());
	std::string						&channelName = message[0];
	Server::channelMap_iterator		it;	
	
	this->_source.message = &message;
	if (this->validChannelPrefix(channelName))
	{
		if ((it = this->channelFind(channelName)) == this->_channelMap.end())
			this->_eventHandler.raise(NOTCHANEVENT, this->_source);
		else
		{
			channel = it->second.second;
			this->_source.channel = channel;
			if ((it = user.channelFind(channelName)) == user.getChannelMap().end())
				this->_eventHandler.raise(NOTINCHANEVENT, this->_source);
			else
			{
				this->_eventHandler.raise(PARTEVENT, this->_source);
				this->_removeUserFromChannel(*channel, user);
			}
		}
	}
	else
		this->_eventHandler.raise(ERRCHANEVENT, this->_source);
}

bool const	&Server::isRegistered(void) const
{
	return this->_registered;
}

void	Server::setRegistered(bool value)
{
	this->_registered = value;
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
		this->addUser(*user);
//		this->send("New user is connected.\r\n");
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

void	Server::_checkUserIO(void)
{
	User	*user;
	size_t	size;

	for (int i = 2; i < MAXUSERS + 2; i++)
	{
		if (this->_pollfds[i].revents & POLLIN)
		{
			user = this->_fdMap[this->_pollfds[i].fd];
			size = user->checkInput(this->_pollfds[i].fd);
			if (size <= 0) // ctrl+c
				this->delUser(*user, "");
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

bool	Server::recvCommand(Message &msg)
{
	ACommand	*command;

	msg.getSender()->setIdleTime(time(NULL));
	if ((command = this->findCommand(msg.getCmd())))
	{
		command->recv(msg);
		return true;
	}
	return false;
}

bool	Server::sendCommand(Message &msg)
{
	ACommand	*command;

	if ((command = this->findCommand(msg.getCmd())))
	{
		if (msg.getReceiver() == NULL)
			msg.setReceiver(msg.getSender());
		command->send(msg);
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
				this->quit("Server shutdown");
		}
	}
}

void	Server::_checkUserTimeout(User &user)
{
	if (user.getNextTimeout() && user.getNextTimeout() < time(NULL))
		this->delUser(user, "Registration timeout");
	else if (!user.getNextTimeout() && (user.getIdleTime() + IDLETIMEOUT < time(NULL)))
	{
//		std::cout << "getIdletime = " << user.getIdleTime() << " : time = " << time(NULL) << std::endl;
		if (user.isRegistered())
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
//	{
		this->_checkUserTimeout(*(it++)->second);
//		user = it->second;
//		it++;
//	}
}

void	Server::_loop(void)
{
	int	rv;

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
