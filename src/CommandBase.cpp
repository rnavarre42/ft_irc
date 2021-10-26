#include "CommandBase.hpp"
#include <string>
#include <iostream>

CommandBase::CommandBase(Server &server, std::string name, char level) : server(server), name(name), level(level)
{
	std::cout << "Nueva instancia de CommandBase, name = \"" << this->name << "\" level = " << static_cast<unsigned>(level) << std::endl;
}

CommandBase::~CommandBase(void)
{}

UserCommand::UserCommand(Server &server, char level) : CommandBase(server, "USER", level)
{}

void UserCommand::exec(void)
{
	std::cout << "al ataque" << std::endl;
}
