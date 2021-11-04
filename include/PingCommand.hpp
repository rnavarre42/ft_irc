#ifndef PINGCOMMAND_HPP
# define PINGCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class PingCommand : public ACommand
{
public:
	PingCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_recvUser(Message &message);
	virtual bool	_recvServer(Message &message);
	virtual bool	_sendUser(Message &message);
	virtual bool	_sendServer(Message &message);
};

#endif
