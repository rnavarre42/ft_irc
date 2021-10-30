#ifndef NOTICECOMMAND_HPP
# define NOTICECOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class NoticeCommand : public ACommand
{
	public:
		NoticeCommand(Server &server, int accessLevel, int paramCount);

	private:
		virtual bool	_exec(Message &message);
};

#endif
