#include "QuitCommand.hpp"
#include <string>
#include <iostream>

void	QuitCommand::exec(Server &server)
{
	(void)server;
	std::cout << "QuitEvent" << std::endl;
}

void	QuitCommand::_load(Server &server)
{
	(void)server;
}
