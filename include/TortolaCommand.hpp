#ifndef TORTOLACOMMAND_HPP
# define TORTOLACOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class TortolaCommand : public ACommand
{
public:
	TortolaCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_exec(Message &message);
};

#endif
