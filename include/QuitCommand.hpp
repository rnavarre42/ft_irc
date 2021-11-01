#ifndef QUITCOMMAND_HPP
# define QUITCOMMAND_HPP

# include "ACommand.hpp"
# include <string>

class QuitCommand : public ACommand
{
public:
	QuitCommand(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_execUser(Message &message);
	virtual bool	_execServer(Message &message);
};

#endif
