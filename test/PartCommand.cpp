#include "PartCommand.hpp"
#include <string>
#include <iostream>

void	PartCommand::exec(Server &server)
{
	(void)server;
	std::cout << "PartEvent" << std::endl;
}

void	PartCommand::_load(Server &server)
{
	(void)server;
}
