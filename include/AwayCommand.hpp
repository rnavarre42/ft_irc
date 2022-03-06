#ifndef AWAYCOMMAND_HPP
# define AWAYCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class AwayCommand : public ACommand
{
public:
	AwayCommand(Server& server, int accessLevel, int paramCount);
	~AwayCommand(void);

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
