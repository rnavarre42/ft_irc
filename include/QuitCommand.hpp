#ifndef QUITCOMMAND_HPP
# define QUITCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class QuitCommand : public ACommand
{
public:
	QuitCommand(Server &server, int accessLevel, int paramCount);
	void loadEvents(Server::eventHandler_type &eventHandler);
	void unloadEvents(Server::eventHandler_type &eventHandler);
	void QuitEvent(Message &message);
	void DelUserEvent(Message &message);

private:
	virtual bool	_recvUser(Message &message);
	virtual bool	_recvServer(Message &message);
	virtual bool	_sendUser(Message &message);
	virtual bool	_sendServer(Message &message);
};

#endif
