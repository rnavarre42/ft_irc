#include "JoinCommand.hpp"
#include <string>
#include <iostream>

void	JoinCommand::exec(Server &server)
{
	(void)server;
	std::cout << "JoinEvent" << std::endl;
}

void	JoinCommand::_load(Server &server)
{
	(void)server;
}
