#ifndef PONGCOMMAND_HPP
# define PONGCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class PongCommand : public ACommand
{
public:
	PongCommand(Server &server, int accessLevel, int paramCount);
	void loadEvents(Server::eventHandler_type &eventHandler);
	void unloadEvents(Server::eventHandler_type &eventHandler);

	void registerUserEvent(Source &source);

private:
	virtual bool	_recvUser(Message &message);
	virtual bool	_recvServer(Message &message);
	virtual bool	_sendUser(Message &message);
	virtual bool	_sendServer(Message &message);
};

#endif
