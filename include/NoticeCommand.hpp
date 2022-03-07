#ifndef NOTICECOMMAND_HPP
# define NOTICECOMMAND_HPP

# include "ACommand.hpp"

# include <string>

class NoticeCommand : public ACommand
{
public:
	NoticeCommand(Server& server, int accessLevel, int paramCount);
	~NoticeCommand(void);

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
