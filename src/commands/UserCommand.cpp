#include "UserCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

UserCommand::UserCommand(Server &server, int accessLevel, int paramCount) : ACommand(server, accessLevel, paramCount)
{}

void UserCommand::_exec(Message &message)
{
	message.getSender().send("Command not implemented yet");
	std::cout << "exec message > " << message.getCmd() << std::endl;
}
