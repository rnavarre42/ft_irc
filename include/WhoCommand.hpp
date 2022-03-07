#ifndef WHOCOMMAND_HPP
# define WHOCOMMAND_HPP

# include "ACommand.hpp"

# include <string>

class WhoCommand : public ACommand
{
public:
	WhoCommand(Server& server, int accessLevel, int paramCount);
	~WhoCommand(void);

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
