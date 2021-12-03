#ifndef QUITCOMMAND_HPP
# define QUITCOMMAND_HPP

# include "CommandBase.hpp"

class	QuitCommand : public CommandBase
{
public:
	void exec(Server &server);

};

#endif
