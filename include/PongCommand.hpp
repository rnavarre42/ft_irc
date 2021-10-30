#ifndef PONGCOMMAND_HPP
# define PONGCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class PongCommand : public ACommand
{
public:
	PongCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_exec(Message &message);
};

#endif
