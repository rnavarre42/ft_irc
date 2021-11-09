#ifndef PARTCOMMAND_HPP
# define PARTCOMMAND_HPP

# include "CommandBase.hpp"

class	PartCommand : public CommandBase
{
public:
	void exec(Server &server);

};

#endif
