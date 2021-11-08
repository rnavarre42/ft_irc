#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

#include "Server.hpp"

class Server;

class ACommand
{
public:
	virtual void	exec(Server &server) = 0;

private:
	virtual void	_load(Server &server) = 0;

	int				_type;
	friend class Server;

protected:
	ACommand(void)
	{}
};

#endif
