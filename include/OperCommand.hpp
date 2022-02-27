#ifndef OPERCOMMAND_HPP
# define OPERCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class OperCommand : public ACommand
{
public:
	OperCommand(Server &server, int accessLevel, int paramCount);
	~OperCommand(void);

	void	loadEvents(Server::eventHandler_type& eventHandler);
	void	unloadEvents(Server::eventHandler_type& eventHandler);

private:
	bool	_recvUser(Message& message);
	bool	_recvServer(Message& message);
	bool	_sendUser(Message& message);
	bool	_sendServer(Message& message);
};

#endif
