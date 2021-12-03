#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class JoinCommand : public ACommand
{
public:
	JoinCommand(Server &server, int accessLevel, int paramCount);
	~JoinCommand(void) {}

	void joinChannelEvent(Message &message);
	void alreadyChannelEvent(Message &message);
	void createChannelEvent(Message &message);
	void errChannelEvent(Message &message);
	void delChannelEvent(Message &message);
	void limitChannelEvent(Message &message);
	void loadEvents(Server::eventHandler_type &eventHandler);
	void unloadEvents(Server::eventHandler_type &eventHandler);

private:
	virtual bool	_recvUser(Message &message);
	virtual bool	_recvServer(Message &message);
	virtual bool	_sendUser(Message &message);
	virtual bool	_sendServer(Message &message);
};

#endif
