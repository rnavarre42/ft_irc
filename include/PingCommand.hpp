#ifndef PINGCOMMAND_HPP
# define PINGCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class PingCommand : public ACommand
{
public:
	PingCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_exec(Message &message);
};

#endif
