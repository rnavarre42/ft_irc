#ifndef ${CLASS_UPPER}COMMAND_HPP
# define ${CLASS_UPPER}COMMAND_HPP

# include "ACommand.hpp"
# include <string>

class ${CLASS}Command : public ACommand
{
public:
	${CLASS}Command(Server &server, int accessLevel, int paramCount);

private:
	virtual bool	_execUser(Message message);
	virtual bool	_execServer(Message message);
};

#endif
