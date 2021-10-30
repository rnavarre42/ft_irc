#ifndef KICKCOMMAND_HPP
# define KICKCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class KickCommand : public ACommand
{
public:
	KickCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_exec(Message &message);
};

#endif
