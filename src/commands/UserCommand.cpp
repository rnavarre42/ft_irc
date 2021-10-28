#include "UserCommand.hpp"
#include "Server.hpp"
#include <iostream>

UserCommand::UserCommand(Server &server, char level) : ACommand(server, level)
{}

void UserCommand::exec(void)
{
	std::cout << "al ataque" << std::endl;
}
