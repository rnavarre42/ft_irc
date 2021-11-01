#ifndef AWAYCOMMAND_HPP
# define AWAYCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class AwayCommand : public ACommand
{
public:
	AwayCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_execUser(Message &message);
	virtual bool	_execServer(Message &message);
};

#endif
