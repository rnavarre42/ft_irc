#ifndef PARTCOMMAND_HPP
# define PARTCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class PartCommand : public ACommand
{
public:
	PartCommand(Server &server, int accessLevel, int paramCount);
	void partChannelEvent(Source &source);
	void notChannelEvent(Source &source);
	void notInChannelEvent(Source &source);
	void loadEvents(Server::eventHandler_type &eventHandler);
	void unloadEvents(Server::eventHandler_type &eventHandler);

private:
	virtual bool	_recvUser(Message &message);
	virtual bool	_recvServer(Message &message);
	virtual bool	_sendUser(Message &message);
	virtual bool	_sendServer(Message &message);
};

#endif
