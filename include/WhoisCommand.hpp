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
	bool	_recvUser(Message& message);
	bool	_recvServer(Message& message);
	bool	_sendUser(Message& message);
	bool	_sendServer(Message& message);
};

#endif
