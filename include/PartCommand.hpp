#ifndef PARTCOMMAND_HPP
# define PARTCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class PartCommand : public ACommand
{
public:
	PartCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_execUser(Message &message);
	virtual bool	_execServer(Message &message);
};

#endif
