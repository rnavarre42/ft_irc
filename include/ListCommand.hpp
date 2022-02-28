#ifndef LISTCOMMAND_HPP
# define LISTCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class ListCommand : public ACommand
{
public:
	ListCommand(Server& server, int accessLevel, int paramCount);
	~ListCommand(void);

	void	loadEvents(Server::eventHandler_type& eventHandler);
	void	unloadEvents(Server::eventHandler_type& eventHandler);

private:
	bool	_recvUser(Message& message);
	bool	_recvServer(Message& message);
	bool	_sendUser(Message& message);
	bool	_sendServer(Message& message);
};

#endif
