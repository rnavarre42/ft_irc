#ifndef KILLCOMMAND_HPP
# define KILLCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class KillCommand : public ACommand
{
public:
	KillCommand(Server& server, int accessLevel, int paramCount);
	~KillCommand(void);

	void	loadEvents(Server::eventHandler_type& eventHandler);
	void	unloadEvents(Server::eventHandler_type& eventHandler);

private:
	bool	_recvUser(Message& message);
	bool	_recvServer(Message& message);
	bool	_recvUnknown(Message& message);
	bool	_sendUser(Message& message);
	bool	_sendServer(Message& message);
};

#endif
