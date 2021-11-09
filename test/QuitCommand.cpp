#include "QuitCommand.hpp"
#include <iostream>

void	QuitCommand::exec(Server &server)
{
	std::cout << "QuitEvent : " << server.getName() << std::endl;
}
