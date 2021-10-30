#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "ACommand.hpp"
# include <string>

class Command : public ACommand
{
public:
	Command(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_exec(Message &message);
};

#endif
