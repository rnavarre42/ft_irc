#ifndef WHOWASCOMMAND_HPP
# define WHOWASCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class WhowasCommand : public ACommand
{
public:
	WhowasCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_exec(Message &message);
};

#endif
