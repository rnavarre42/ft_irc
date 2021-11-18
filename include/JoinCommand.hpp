#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class JoinCommand : public ACommand
{
public:
	JoinCommand(Server &server, int accessLevel, int paramCount);

	void joinChannelEvent(Source &source);
	void alreadyChannelEvent(Source &source);
	void createChannelEvent(Source &source);
	void errChannelEvent(Source &source);
	void delChannelEvent(Source &source);
	void limitChannelEvent(Source &source);
	void loadEvents(Server::eventHandler_type &eventHandler);
	void unloadEvents(Server::eventHandler_type &eventHandler);

private:
	virtual bool	_recvUser(Message &message);
	virtual bool	_recvServer(Message &message);
	virtual bool	_sendUser(Message &message);
	virtual bool	_sendServer(Message &message);
};

#endif
