#ifndef MODECOMMAND_HPP
# define MODECOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class ModeCommand : public ACommand
{
public:
	ModeCommand(Server& server, int accessLevel, int paramCount);
	~ModeCommand(void);

	void	loadEvents(Server::eventHandler_type& eventHandler);
	void	unloadEvents(Server::eventHandler_type& eventHandler);

private:
	bool	_recvUser(Message& message);
	bool	_recvServer(Message& message);
	bool	_recvUnknown(Message& message);
	bool	_sendUser(Message& message);
	bool	_sendServer(Message& message);
	bool	_sendUnknown(Message& message);

	void	_checkChanModes(Message& message);
	void	_checkUserModes(Message& message);
};

#endif
