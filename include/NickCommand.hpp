#ifndef NICKCOMMAND_HPP
# define NICKCOMMAND_HPP

# include "ACommand.hpp"

# include <string>

class NickCommand : public ACommand
{
public:
	NickCommand(Server &server, int accessLevel, int paramCount);
	~NickCommand(void);

	void	loadEvents(Server::eventHandler_type& eventHandler);
	void	unloadEvents(Server::eventHandler_type& eventHandler);

	void	nickEvent(Message& message);

private:
	bool	_recvUser(Message& message);
	bool	_recvServer(Message& message);
	bool	_recvUnknown(Message& message);
	bool	_sendUser(Message& message);
	bool	_sendServer(Message& message);
};

#endif
