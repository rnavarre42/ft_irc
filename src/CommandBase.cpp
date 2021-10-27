#include "CommandBase.hpp"
#include <string>
#include <iostream>

CommandBase::CommandBase(Server &server, char level) : server(server), level(level)
{
//	std::cout << "Nueva instancia de CommandBase, level = " << static_cast<unsigned>(level) << std::endl;
}

CommandBase::~CommandBase(void)
{}
