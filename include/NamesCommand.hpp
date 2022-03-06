#ifndef NAMESCOMMAND_HPP
# define NAMESCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class NamesCommand : public ACommand
{
public:
	NamesCommand(Server &server, int accessLevel, int paramCount);
	~NamesCommand(void);

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
