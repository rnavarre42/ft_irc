#ifndef PRIVATEMSGCOMMAND_HPP
# define PRIVATEMSGCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class PrivmsgCommand : public ACommand
{
public:
	PrivmsgCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_exec(Message &message);
};

#endif
