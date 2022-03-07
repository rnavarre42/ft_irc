#ifndef WHOWASCOMMAND_HPP
# define WHOWASCOMMAND_HPP

# include "ACommand.hpp"

# include <string>

class WhowasCommand : public ACommand
{
public:
	WhowasCommand(Server& server, int accessLevel, int paramCount);
	~WhowasCommand(void);

	void	loadEvents(Server::eventHandler_type& eventHandler);
	void	unloadEvents(Server::eventHandler_type& eventHandler);

private:
	bool	_recvUser(Message& message);
	bool	_recvServer(Message& message);
	bool	_recvUnknown(Message& message);
	bool	_sendUser(Message& message);
	bool	_sendServer(Message& message);
	bool	_sendUnknown(Message& message);
};

#endif
