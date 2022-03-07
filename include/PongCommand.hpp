#ifndef PONGCOMMAND_HPP
# define PONGCOMMAND_HPP

# include "ACommand.hpp"

# include <string>

class PongCommand : public ACommand
{
public:
	PongCommand(Server &server, int accessLevel, int paramCount);
	~PongCommand(void);

	void	registerUserEvent(Message& message);

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
