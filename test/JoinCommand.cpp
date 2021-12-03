#include "JoinCommand.hpp"
#include <iostream>

void	JoinCommand::exec(Server &server)
{
	std::cout << "JoinEvent : " << server.getName() << std::endl;
}
