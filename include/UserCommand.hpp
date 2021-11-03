#ifndef USERCOMMAND_HPP
# define USERCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class UserCommand : public ACommand
{
public:
	UserCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_execUser(Message &message);
	virtual bool	_execServer(Message &message);
};

#endif