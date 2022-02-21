#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Server.hpp"

# include <string>

# define LEVEL_UNREGISTERED	0x01
# define LEVEL_NEGOTIATING	0x02
# define LEVEL_REGISTERED	0x04
# define LEVEL_IRCOPERATOR	0x08
# define LEVEL_ALL			LEVEL_UNREGISTERED | LEVEL_NEGOTIATING | LEVEL_REGISTERED | LEVEL_IRCOPERATOR

//class Server;
class Message;
class User;

template <class EvenT, class ValueT> 
class EventHandler;

class ACommand
{
public:
	ACommand(Server &server, int levelAccess, int minParam);
	virtual ~ACommand(void);
	virtual void loadEvents(Server::eventHandler_type &eventHandler) = 0;
	virtual void unloadEvents(Server::eventHandler_type &eventHandler) = 0;

	void	recv(Message &msg);
	void	send(Message &msg);

private:
	ACommand(void);
	ACommand(ACommand const &src);
	ACommand &operator=(ACommand const &rhs);

	virtual bool	_recvUser(Message &message) = 0;
	virtual bool	_recvServer(Message &message) = 0;
	virtual bool	_sendUser(Message &message) = 0;
	virtual bool	_sendServer(Message &message) = 0;

protected:
	Server		&server;
	int			levelAccess;
	size_t		minParam;
	Server		*serverSender;
	User		*userSender;
	Server		*serverReceiver;
	User		*userReceiver;
};

#endif
