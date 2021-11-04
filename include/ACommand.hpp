#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Server.hpp"
# include "User.hpp"
# include "Message.hpp"
# include <string>

# define LEVEL_UNREGISTERED	0x00
# define LEVEL_REGISTERED	0x01
# define LEVEL_IRCOPERATOR	0x02
# define LEVEL_ALL			0x03

class Server;
class Message;
class User;

class ACommand
{
public:
	ACommand(Server &server, int accessLevel, int minParam);
	~ACommand(void);

	void	recv(Message &msg);
	void	send(Message &msg);

private:
	virtual bool	_recvUser(Message &message) = 0;
	virtual bool	_recvServer(Message &message) = 0;
	virtual bool	_sendUser(Message &message) = 0;
	virtual bool	_sendServer(Message &message) = 0;

protected:
	Server		&server;
	int			accessLevel;
	size_t		minParam;
	Server		*serverSender;
	User		*userSender;
	Server		*serverReceiver;
	User		*userReceiver;
};

#endif
