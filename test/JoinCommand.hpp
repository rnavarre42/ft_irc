#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "CommandBase.hpp"

class	JoinCommand : public CommandBase
{
public:
	void exec(Server &server);

};

#endif
