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
	ACommand(Server &server, int accessLevel, int paramCount);
	~ACommand(void);

	void exec(Message &msg);

private:
	virtual bool	_execUser(Message &msg) = 0;
	virtual bool	_execServer(Message &msg) = 0;

protected:
	Server		&server;
	int			level;
	size_t		count;
	Server		*serverSender;
	User		*userSender;
};

#endif
