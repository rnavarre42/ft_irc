#ifndef KILLCOMMAND_HPP
# define KILLCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class KillCommand : public ACommand
{
public:
	KillCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_execUser(Message &message);
	virtual bool	_execServer(Message &message);
};

#endif
