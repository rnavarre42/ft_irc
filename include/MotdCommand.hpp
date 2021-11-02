#ifndef MOTDCOMMAND_HPP
# define MOTDCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class MotdCommand : public ACommand
{
public:
	MotdCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_execUser(Message &message);
	virtual bool	_execServer(Message &message);
};

#endif
