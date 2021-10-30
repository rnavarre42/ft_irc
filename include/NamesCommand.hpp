#ifndef NAMESCOMMAND_HPP
# define NAMESCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class NamesCommand : public ACommand
{
public:
	NamesCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_exec(Message &message);
};

#endif
