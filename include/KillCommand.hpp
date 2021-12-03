#ifndef KILLCOMMAND_HPP
# define KILLCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class KillCommand : public ACommand
{
public:
	KillCommand(Server &server, int accessLevel, int paramCount);
	void loadEvents(Server::eventHandler_type &eventHandler);
	void unloadEvents(Server::eventHandler_type &eventHandler);

private:
	virtual bool	_recvUser(Message &message);
	virtual bool	_recvServer(Message &message);
	virtual bool	_sendUser(Message &message);
	virtual bool	_sendServer(Message &message);
};

#endif
