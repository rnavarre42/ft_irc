#include "Server.hpp"
#include "Channel.hpp"
#include "Console.hpp"
#include "Message.hpp"
#include "utils.hpp"
#include "commands.hpp"

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

//template <typename T>
//class EventHandler;

Server	*Server::instance = NULL;

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
	: ip(listenIp), port(listenPort), name(name), type(TYPE_SERVER)
{
	this->_loadCommands();
//	this->_logger();
//	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, Server::signalHandler);
	std::memset(this->pollfds, '\0', sizeof(struct pollfd) * (MAXUSERS + 2));
	this->stop = false;
	this->initSocket();
	this->pollTimeout = 1000;
	this->registered = 0;
	this->_bind();
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
//	this->_commandMap["PRIVMSG"]	= new PrivmsgCommand(*this, LEVEL_REGISTERED, 1);
//	this->_commandMap["QUIT"]	= new QuitCommand	(*this, LEVEL_ALL, 0);
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

	std::map<std::string, ACommand *>::iterator	it;

	for (it = this->_commandMap.begin(); it != this->_commandMap.end(); it++)
		it->second->loadEvents(this->_eventHandler);
		

//	iterar load(this->_eventHandler);
}

void	Server::_unloadCommands(void)
{
	std::map<std::string, ACommand *>::iterator	it;

	for (it = this->_commandMap.begin(); it != this->_commandMap.end(); it++)
		delete it->second;
}

Server	&Server::getInstance(void)
{
	return *Server::instance;
}
Server	&Server::getInstance(std::string listenIp, int listenPort, std::string name)
{
	if (Server::instance == NULL)
		Server::instance = new Server(listenIp, listenPort, name);
	return *Server::instance;
}

void	Server::deleteInstance(void)
{
	delete Server::instance;
}

std::map<std::string, User *>	&Server::getUserMap(void)
{
	return this->_userMap;
}

int		Server::count(void)
{
	return this->_fdMap.size();
}

int	const	&Server::getFd(void) const
{
	return this->fd;
}

void	Server::setPass(std::string value)
{
	this->pass = value;
}

std::string	const	&Server::getPass(void) const
{
	return this->pass;
}

void	Server::setPollout(User &user)
{
	this->pollfds[user.getPollIndex()].events |= POLLOUT;
}

ssize_t	Server::send(std::string msg)
{
	//int usersLeft;
	Server::_fdMap_iterator	it;

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
	message.getSender().setRegistered(true);
	this->_eventHandler.raise(REGUSEREVENT, message);
}

void	Server::quit(std::string msg)
{
	Server::_fdMap_iterator	currentIt;

	for (Server::_fdMap_iterator it = this->_fdMap.begin(); it != this->_fdMap.end();)
	{
		currentIt = it;
		it++;
		currentIt->second->send(msg);
		this->delUser(*currentIt->second);
	}
	this->stop = true;
}

void	Server::start(void)
{
	this->_listen();
	this->loop();
}

void	Server::initSocket(void)
{
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (!this->fd)
	{
		Console::log(LOG_ERROR, "Server::initSocket function socket() failed");
		exit(EXIT_FAILURE);
	}
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) == -1)
	{
		Console::log(LOG_ERROR, "Server::initSocket function fcntl() failed");
		exit(EXIT_FAILURE);
	}
	this->opt = 1;
	this->addrlen = sizeof(this->address);
	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &this->opt, sizeof(this->opt)))
	{
		Console::log(LOG_ERROR, "Server::initSocket function setsockopt() failed");
		exit(EXIT_FAILURE);
	}
}

int		Server::findFreePollIndex(void)
{
	for (int i = 2; i < MAXUSERS + 2; i++)
	{
		if (this->pollfds[i].fd == 0)
			return i;
	}
	return 0;
}

std::string const	&Server::getName(void) const
{
	return this->name;
}

std::string			Server::getMask(void)
{
	return this->name;
}

bool	Server::isUser(void)
{
	return this->type == TYPE_USER;
}

bool	Server::isServer(void)
{
	return this->type == TYPE_SERVER;
}

bool	Server::isOper(void)
{
	return false;
}

int	Server::getType(void)
{
	return this->type;
}
void	Server::setIdleTime(time_t value)
{
	this->idleTime = value;
}

User	*Server::_accept(void)
{
	User	*user;
	int		newFd;

	newFd = accept(this->fd, (struct sockaddr *)&this->address, (socklen_t *)&this->addrlen);
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
	user->setPollIndex(findFreePollIndex());
//	std::cerr << "setPollIndex = " << user->getPollIndex() << std::endl;
	this->pollfds[user->getPollIndex()].fd = newFd;
	this->pollfds[user->getPollIndex()].events = POLLIN;
	return user;
}

void	Server::_bind(void)
{
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(this->port);
	if (bind(this->fd, (struct sockaddr *)&this->address, sizeof(this->address)) < 0)
	{
		Console::log(LOG_ERROR, "Server::bind function bind() failed");
		exit(EXIT_FAILURE);
	}
}

void	Server::_listen(void)
{
	if (listen(this->fd, MAXLISTEN) < 0)
	{
		Console::log(LOG_ERROR, "Server::listen function listen() failed");
		exit(EXIT_FAILURE);
	}
	this->pollfds[0].fd = this->fd;
	this->pollfds[0].events = POLLIN;
	this->pollfds[1].fd = 0;
	this->pollfds[1].events = POLLIN;
}

int		Server::_poll(void)
{
	return poll(this->pollfds, MAXUSERS + 2, this->pollTimeout);
}

void	Server::addUser(User &user)
{
	Message message = Message::builder(*this);
	message.setReceiver(&user);
//	this->userVector.push_back(&user);
	this->_fdMap[user.getFd()] = &user;
	this->_eventHandler.raise(NEWUSEREVENT, message);
}

void	Server::delUser(User &user)
{
	Message message = Message::builder(*this);
	message.setReceiver(&user);
	this->_eventHandler.raise(DELUSEREVENT, message);
	// Se elimina de userMap si tiene nick
	if (user.getName().empty())
		;
//		this->send("User <anonymous> disconnect\n");
	else
	{
//		this->send("User <" + user.getName() + "> disconnect\n");
		this->_userMap.erase(strToUpper(user.getName()));
	}
	this->_fdMap.erase(user.getFd());
	this->pollfds[user.getPollIndex()].fd = 0;
	this->pollfds[user.getPollIndex()].events = 0;
//	this->userVector.erase(std::remove(this->userVector.begin(), this->userVector.end(), &user), this->userVector.end());
	delete &user;
}

Server::userMap_iterator	Server::_userFind(std::string &userName)
{
	return this->_userMap.find(strToUpper(userName));
}

Server::channelMap_iterator	Server::_channelFind(std::string &channelName)
{
	return this->_channelMap.find(strToUpper(channelName));
}

// :masksource JOIN #CHAN :Pass
Channel *Server::addToChannel(Message &message)
{
	Channel											*channel = NULL;
	std::string										&channelName = message[0];
	User											&user = static_cast<User &>(message.getSender());
	Server::channelMap_iterator						it;
	std::pair<Server::userMap_iterator, bool>		retUser;

	if (channelName[0] == '#')
	{
		if ((it = this->_channelFind(channelName)) == this->_channelMap.end())
		{
			channel = new Channel(channelName, user);
			this->_channelMap[strToUpper(channelName)] = channel;
			channel->getUserMap()[strToUpper(user.getName())] = &user;
			user.getChannelMap()[strToUpper(channelName)] = channel;
			this->_eventHandler.raise(NEWCHANEVENT, message);
			//this->_eventHandler.raise(JOINEVENT, message);
		}
		else
		{
			channel = it->second;
			retUser = channel->getUserMap().insert(std::pair<std::string, User *>(strToUpper(user.getName()), &user));
			if (retUser.second == true)  //El nick ha entrado al canal
				this->_eventHandler.raise(JOINEVENT, message);
				//TODO informar al usuario de que ha entrado y al resto.
			else
				this->_eventHandler.raise(ALREADYEVENT, message);
			//TODO falta por gestionar +l +i +k
		}
	}
	return channel;
}

int		Server::delFromChannel(Message &message)
{
	(void)message;
	return false;
}

bool const	&Server::isRegistered(void) const
{
	return this->registered;
}

void	Server::setRegistered(bool value)
{
	this->registered = value;
}

int	Server::checkUserConnection(void)
{
	User	*user;

	if (this->pollfds[0].revents & POLLIN)
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

void	Server::checkUserIO(void)
{
	User	*user;
	size_t	size;

	for (int i = 2; i < MAXUSERS + 2; i++)
	{
		if (this->pollfds[i].revents & POLLIN)
		{
			user = this->_fdMap[this->pollfds[i].fd];
			size = user->checkInput(this->pollfds[i].fd);
			if (size <= 0)
				this->delUser(*user);
		}
		else if (this->pollfds[i].revents & POLLOUT)
		{
			user = this->_fdMap[this->pollfds[i].fd];
		//	std::cout << "El usuario " << user->getName() << " ya acepta mensajes" << std::endl;
			if (user->checkOutput(this->pollfds[i].fd))
				this->pollfds[i].events ^= POLLOUT;
		}
		else if (this->pollfds[i].fd > 0)
		{
			user = this->_fdMap[this->pollfds[i].fd];
			this->checkUserTimeout(*user);
		}
	}
}

ACommand	*Server::findCommand(std::string cmd)
{
	if (this->_commandMap.find(cmd) != this->_commandMap.end())
		return this->_commandMap[cmd];
	return NULL;
}

bool	Server::recvCommand(Message &msg)
{
	ACommand	*command;

	msg.getSender().setIdleTime(time(NULL));
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
			msg.setReceiver(&msg.getSender());
		command->send(msg);
		return true;
	}
	return false;
}
void	Server::checkConsoleInput(void)
{
	int		size;
	char	buffer[BUFFERSIZE + 1];
	std::string	str = "console> ";

	if (this->pollfds[1].revents & POLLIN)
	{
		size = read(0, buffer, BUFFERSIZE);
		buffer[size] = '\0';
		if (size > 0)
		{
			if (!strcmp(buffer, "quit\n"))
				this->stop = true;
		}
	}
}

void	Server::checkUserTimeout(User &user)
{
	Message	*msg;

	if (user.getNextTimeout() && user.getNextTimeout() < time(NULL))
	{
		msg = &Message::builder(*this);
		msg->setCmd("QUIT");
		msg->setReceiver(&user);
		msg->insertField("Ping timeout");
		this->sendCommand(*msg);
		delete msg;
	}
	else if (!user.getNextTimeout() && (user.getIdleTime() + IDLETIMEOUT < time(NULL)))
	{
//		std::cout << "getIdletime = " << user.getIdleTime() << " : time = " << time(NULL) << std::endl;
		if (user.isRegistered())
		{
			user.setNextTimeout(time(NULL) + NEXTTIMEOUT);
			user.setPingChallenge(this->name);
		}
		user.send("PING :" + user.getPingChallenge());
	}
}

void	Server::checkTimeout(void)
{
	Server::_fdMap_iterator	it;

	for (it = this->_fdMap.begin(); it != this->_fdMap.end();)
//	{
		checkUserTimeout(*(it++)->second);
//		user = it->second;
//		it++;
//	}
}

void	Server::loop(void)
{
	int	rv;

	while (!this->stop)
	{
		rv = this->_poll();
		if (rv == -1)
		{
			if (this->stop)
			{
				Console::log(LOG_ERROR, "Server stopped by user");
				exit(0);
			}
			Console::log(LOG_ERROR, "Server::loop function poll() failed");
			exit(EXIT_FAILURE);
		}
		else if (rv == 0)
			this->checkTimeout();
		else
		{
			if (!this->checkUserConnection())
				this->checkUserIO();
			this->checkConsoleInput();
		}
	}
}
