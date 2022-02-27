#ifndef WHOISCOMMAND_HPP
# define WHOISCOMMAND_HPP

# include "ACommand.hpp"

# include <string>

class WhoisCommand : public ACommand
{
public:
	WhoisCommand(Server& server, int accessLevel, int paramCount);
	~WhoisCommand(void);

	void	loadEvents(Server::eventHandler_type& eventHandler);
	void	unloadEvents(Server::eventHandler_type& eventHandler);

private:
	bool	_execUser(Message& message);
	bool	_execServer(Message& message);
};

#endif
