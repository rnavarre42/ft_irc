#ifndef USERCOMMAND_HPP
# define USERCOMMAND_HPP

# include "ACommand.hpp"

class UserCommand : public ACommand
{
public:
	UserCommand(Server& server, int accessLevel, int paramCount);
	~UserCommand(void);

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
