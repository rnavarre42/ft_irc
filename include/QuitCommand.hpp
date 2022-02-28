#ifndef QUITCOMMAND_HPP
# define QUITCOMMAND_HPP

# include "ACommand.hpp"

# include <string>

class QuitCommand : public ACommand
{
public:
	QuitCommand(Server& server, int accessLevel, int paramCount);
	~QuitCommand(void);

	void	quitEvent(Message& message);
	void	delUserEvent(Message& message);

	void	loadEvents(Server::eventHandler_type& eventHandler);
	void	unloadEvents(Server::eventHandler_type& eventHandler);

private:
	bool	_recvUser(Message& message);
	bool	_recvServer(Message& message);
	bool	_sendUser(Message& message);
	bool	_sendServer(Message& message);
};

#endif
