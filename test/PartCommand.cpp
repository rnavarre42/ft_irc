#include "PartCommand.hpp"
#include <iostream>

void	PartCommand::exec(Server &server)
{
	std::cout << "PartEvent : " << server.getName() << std::endl;
}
