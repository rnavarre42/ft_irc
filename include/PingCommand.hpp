#ifndef PINGCOMMAND_HPP
# define PINGCOMMAND_HPP

# include "ACommand.hpp"

# include <string>

class PingCommand : public ACommand
{
public:
	PingCommand(Server &server, int accessLevel, int paramCount);
	~PingCommand(void);

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
