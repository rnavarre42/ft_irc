#ifndef WHOWASCOMMAND_HPP
# define WHOWASCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class WhowasCommand : public ACommand
{
public:
	WhowasCommand(Server &server, int accessLevel, int paramCount);
	void loadEvents(Server::eventHandler_type &eventHandler);
	void unloadEvents(Server::eventHandler_type &eventHandler);

private:
	virtual bool	_execUser(Message &message);
	virtual bool	_execServer(Message &message);
};

#endif
