#ifndef MODECOMMAND_HPP
# define MODECOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class ModeCommand : public ACommand
{
public:
	ModeCommand(Server& server, int accessLevel, int paramCount);
	void loadEvents(Server::eventHandler_type &eventHandler);
	void unloadEvents(Server::eventHandler_type &eventHandler);

private:
	virtual bool	_recvUser(Message& message);
	virtual bool	_recvServer(Message& message);
	virtual bool	_sendUser(Message& message);
	virtual bool	_sendServer(Message& message);

	void			_checkChanModes(Message& message);
	void			_checkUserModes(Message& message);
};

#endif
