#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Server.hpp"
# include "Message.hpp"
# include <string>

# define LEVEL_UNREGISTERED	0x00
# define LEVEL_REGISTERED	0x01
# define LEVEL_ALL			0x02

class Server;
class Message;

class ACommand
{
public:
	ACommand(Server &server, int accessLevel, int paramCount);
	~ACommand(void);

	void exec(Message &msg);

private:
	virtual bool	_exec(Message &msg) = 0;

protected:
	Server		&server;
	int			level;
	int			count;
};

#endif
