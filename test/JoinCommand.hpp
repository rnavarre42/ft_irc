#ifndef JOINCOMMAND_HPP
# define JOINCOMMAND_HPP

# include "ACommand.hpp"

class	JoinCommand : public ACommand
{
public:
	friend class Server;

	void	exec(Server &server);

private:
	void	_load(Server &server);
};

#endif
