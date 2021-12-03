#ifndef COMMANDBASE_HPP
# define COMMANDBASE_HPP

#include "Server.hpp"

class Server;

class CommandBase
{
public:
	virtual void exec(Server &server) = 0;
};

#endif
