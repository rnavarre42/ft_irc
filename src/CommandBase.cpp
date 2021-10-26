#include "CommandBase.hpp"
#include <string>
#include <iostream>

CommandBase::CommandBase(Server &server, char level) : server(server), level(level)
{
	std::cout << "Nueva instancia de CommandBase, level = " << static_cast<unsigned>(level) << std::endl;
}

CommandBase::~CommandBase(void)
{}

UserCommand::UserCommand(Server &server, char level) : CommandBase(server, level)
{}

void UserCommand::exec(void)
{
	std::cout << "al ataque" << std::endl;
}
