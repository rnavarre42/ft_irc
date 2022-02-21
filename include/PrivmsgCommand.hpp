#ifndef PRIVMSGCOMMAND_HPP
# define PRIVMSGCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class PrivmsgCommand : public ACommand
{
public:
	PrivmsgCommand(Server &server, int accessLevel, int paramCount);
	void loadEvents(Server::eventHandler_type &eventHandler);
	void unloadEvents(Server::eventHandler_type &eventHandler);

private:
	bool	_recvUser(Message &message);
	bool	_recvServer(Message &message);
	bool	_sendUser(Message &message);
	bool	_sendServer(Message &message);
};

#endif
