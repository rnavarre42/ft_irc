#ifndef WHOCOMMAND_HPP
# define WHOCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class WhoCommand : public ACommand
{
public:
	WhoCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_exec(Message &message);
};

#endif
