#ifndef WHOISCOMMAND_HPP
# define WHOISCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class WhoisCommand : public ACommand
{
public:
	WhoisCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_exec(Message &message);
};

#endif
