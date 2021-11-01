#ifndef LISTCOMMAND_HPP
# define LISTCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class ListCommand : public ACommand
{
public:
	ListCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_execUser(Message &message);
	virtual bool	_execServer(Message &message);
};

#endif
