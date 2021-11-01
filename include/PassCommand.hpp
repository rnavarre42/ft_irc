#ifndef PASSCOMMAND_HPP
# define PASSCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class PassCommand : public ACommand
{
public:
	PassCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_execUser(Message &message);
	virtual bool	_execServer(Message &message);
};

#endif
