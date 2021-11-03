#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <exception>
#include <csignal>
#include <cstdlib>
#include "Server.hpp"
#include "User.hpp"
#include "commands.hpp"
#include "Message.hpp"
#include "Console.hpp"
#include "Numeric.hpp"
#include "numerics.hpp"
#include "utils.hpp"

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
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, Server::signalHandler);
	std::memset(this->pollfds, '\0', sizeof(struct pollfd) * (MAXUSERS + 2));
	this->stop = false;
	this->initSocket();
	this->pollTimeout = 1000;
	this->registered = 0;
	this->_bind();
}

Server::~Server(void)
{}

void	Server::_loadCommands(void)
{
	this->commandMap["NICK"]	= new NickCommand	(*this, LEVEL_ALL, 1);
	this->commandMap["QUIT"]	= new QuitCommand	(*this, LEVEL_ALL, 1);
	this->commandMap["PONG"]	= new PongCommand	(*this, LEVEL_ALL, 1);
	this->commandMap["PASS"]	= new PassCommand	(*this, LEVEL_UNREGISTERED, 1);
	this->commandMap["USER"]	= new UserCommand	(*this, LEVEL_UNREGISTERED, 4);
	this->commandMap["WHO"]		= new WhoCommand	(*this, LEVEL_REGISTERED, 1);
	this->commandMap["JOIN"]	= new JoinCommand	(*this, LEVEL_REGISTERED, 1);
	this->commandMap["PART"]	= new PartCommand	(*this, LEVEL_REGISTERED, 1);
	this->commandMap["KICK"]	= new KickCommand	(*this, LEVEL_REGISTERED, 2);
	this->commandMap["KILL"]	= new KillCommand	(*this, LEVEL_IRCOPERATOR, 2);
	this->commandMap["AWAY"]	= new AwayCommand	(*this, LEVEL_REGISTERED, 2);
	this->commandMap["LIST"]	= new ListCommand	(*this, LEVEL_REGISTERED, 0);
	this->commandMap["PING"]	= new PingCommand	(*this, LEVEL_REGISTERED, 1);
	this->commandMap["MODE"]	= new ModeCommand	(*this, LEVEL_REGISTERED, 1);
	this->commandMap["WHOIS"]	= new WhoisCommand	(*this, LEVEL_REGISTERED, 1);
	this->commandMap["INVITE"]	= new InviteCommand	(*this, LEVEL_REGISTERED, 2);
	this->commandMap["NOTICE"]	= new NoticeCommand	(*this, LEVEL_REGISTERED, 2);
	this->commandMap["PRIVMSG"]	= new PrivmsgCommand(*this, LEVEL_REGISTERED, 1);
	this->commandMap["WHOWAS"]	= new WhowasCommand	(*this, LEVEL_REGISTERED, 1);
	this->commandMap["NAMES"]	= new NamesCommand	(*this, LEVEL_REGISTERED, 1);
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

std::map<std::string, User *>	&Server::getUserMap(void)
{
	return this->userMap;
}

int		Server::count(void)
{
	return this->fdMap.size();
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
	int usersLeft;

	usersLeft = this->fdMap.size();
	for (size_t i = 1; usersLeft; i++)
	{
		if (this->pollfds[i].fd)
		{
			this->fdMap[this->pollfds[i].fd]->send(msg);
			usersLeft--;
		}
	}

//	for (std::vector<User *>::iterator it = userVector.begin(); it != userVector.end(); it++)
//		it->sendTo(msg);
	return 0;
}

void	Server::quit(std::string msg)
{
//	for (std::vector<User *>::iterator it = userVector.begin(); it != userVector.end(); it++)
//	{
//		it->sendTo(msg);
//		it->disconnect();
//	}
	int usersLeft;

	usersLeft = this->fdMap.size();
	for (size_t i = 1; usersLeft; i++)
	{
		if (this->pollfds[i].fd)
		{
			this->fdMap[this->pollfds[i].fd]->send(msg);
			this->_delUser(*this->fdMap[this->pollfds[i].fd]);
			usersLeft--;
		}
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
	return (this->type == TYPE_USER);
}

bool	Server::isServer(void)
{
	return (this->type == TYPE_SERVER);
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
	if (this->fdMap.size() == MAXUSERS)
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

void	Server::_addUser(User &user)
{
//	this->userVector.push_back(&user);
	this->fdMap[user.getFd()] = &user;
}

void	Server::_delUser(User &user)
{
	if (user.getName().empty())
		;
//		this->send("User <anonymous> disconnect\n");
	else
	{
//		this->send("User <" + user.getName() + "> disconnect\n");
		this->userMap.erase(strToUpper(user.getName()));
	}
	this->fdMap.erase(user.getFd());
	this->pollfds[user.getPollIndex()].fd = 0;
	this->pollfds[user.getPollIndex()].events = 0;
//	this->userVector.erase(std::remove(this->userVector.begin(), this->userVector.end(), &user), this->userVector.end());
	delete &user;
}

bool const	&Server::isRegistered(void) const
{
	return this->registered;
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
		this->_addUser(*user);
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

	for (int pos = 2; pos < MAXUSERS + 2; pos++)
	{
		if (this->pollfds[pos].revents & POLLIN)
		{
			user = this->fdMap[pollfds[pos].fd];
			size = user->checkInput(pollfds[pos].fd);
			if (size <= 0)
				this->_delUser(*user);
		}
		if (this->pollfds[pos].revents & POLLOUT)
		{
			user = this->fdMap[pollfds[pos].fd];
			std::cout << "El usuario " << user->getName() << " ya acepta mensajes" << std::endl;
			if (!user->checkOutput(pollfds[pos].fd))
				this->pollfds[pos].events ^= POLLOUT;
		}
	}
}

bool	Server::findCommand(Message &msg)
{
	if (this->commandMap.find(msg.getCmd()) != commandMap.end())
	{
		commandMap[msg.getCmd()]->exec(msg);
		return true;
	}
	return false;
}

void	Server::checkUserInput(void)
{
	size_t	size;
	size_t	pos;
	char	buffer[BUFFERSIZE + 1];
	std::string	msg;
	User	*user;


	/*	leemos entrada, hasta 512, procesamos mensajes independientes hasta el final.
	 *  solo los que están con fin de linea.
	 *  procesmos 2 mensajes, quedan 480bytes
	 *	--> fd pendiente de leer.
	 *	vuelvo y leo el siguiente paquete. descarto hasta el \r\n y mando los 480 a procesar.
	*/

	for (int i = 2; i < MAXUSERS + 2; i++)
	{
		if (this->pollfds[i].revents & POLLIN)
		{
			user = this->fdMap[pollfds[i].fd];
			size = recv(pollfds[i].fd, buffer, BUFFERSIZE, 0);
			buffer[size] = '\0';
			user->getInputBuffer() += buffer;
			while ((pos = user->getInputBuffer().find('\n')) != std::string::npos)
			{
				msg = user->getInputBuffer().substr(0, pos);
				user->getInputBuffer().erase(0, pos + 1);
				while ((pos = msg.find('\r')) != std::string::npos)
				{
					msg.erase(pos, 1);
				}
				Message &message = Message::messageBuilder(*user, msg);
				if (!message.empty())
				{
					if (this->commandMap.find(message.getCmd()) != commandMap.end())
						commandMap[message.getCmd()]->exec(message);
					else
						user->send(Numeric::builder(*this, *user, ERR_UNKNOWNCOMMAND, (std::string[]){message.getCmd()}, 1));
				}
				delete &message;
			}
			if (size <= 0)
				this->_delUser(*user);
		//	else
		//		this->send(user->getName() + "> " + buffer);
		}
		if (this->pollfds[i].revents & POLLOUT)
		{
			std::cout << "El usuario " << user->getName() << " ya acepta mensajes" << std::endl;
			user = this->fdMap[pollfds[i].fd];
			size = ::send(pollfds[i].fd, user->getOutputBuffer().c_str(), user->getOutputBuffer().size(), 0);
			if (size == user->getOutputBuffer().size())
				this->pollfds[i].events ^= POLLOUT;
			else
				user->getOutputBuffer().erase(0, size);
		}
	}
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
			this->send(buffer);
	}
}

void	Server::checkTimeout(void)
{
	std::map<int, User *>::iterator	it;
	User							*user;

	for (it = this->fdMap.begin(); it != this->fdMap.end();)
	{
		user = it->second;
		it++;
		if (user->getNextTimeout() && user->getNextTimeout() < time(NULL))
		{
			user->send("ERROR :Ping timeout");
			this->_delUser(*user);
		}
		else if (!user->getNextTimeout() && (user->getIdleTime() + IDLETIMEOUT < time(NULL)))
		{
			if (user->isRegistered())
			{
				user->setNextTimeout(time(NULL) + NEXTTIMEOUT);
				user->setPingChallenge(this->name);
			}
			user->send("PING :" + user->getPingChallenge());
		}
	}
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
