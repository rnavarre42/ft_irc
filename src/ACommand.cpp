#include "ACommand.hpp"
#include <string>
#include <iostream>

ACommand::ACommand(Server &server, char level) : server(server), level(level)
{
//	std::cout << "Nueva instancia de CommandBase, level = " << static_cast<unsigned>(level) << std::endl;
}

ACommand::~ACommand(void)
{}
