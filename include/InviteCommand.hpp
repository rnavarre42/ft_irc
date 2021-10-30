#ifndef INVITECOMMAND_HPP
# define INVITECOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class InviteCommand : public ACommand
{
public:
	InviteCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_exec(Message &message);
};

#endif
