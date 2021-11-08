#ifndef PARTCOMMAND_HPP
# define PARTCOMMAND_HPP

# include "ACommand.hpp"

class	PartCommand : public ACommand
{
public:
	friend class Server;

	void	exec(Server &server);

private:
	void	_load(Server &server);
};

#endif
