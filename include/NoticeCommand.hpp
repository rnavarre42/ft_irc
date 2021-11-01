#ifndef NOTICECOMMAND_HPP
# define NOTICECOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class NoticeCommand : public ACommand
{
public:
	NoticeCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_execUser(Message &message);
	virtual bool	_execServer(Message &message);
};

#endif
