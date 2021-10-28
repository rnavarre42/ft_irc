#include "UserCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

UserCommand::UserCommand(Server &server, char level) : ACommand(server, level)
{}

void UserCommand::exec(Message &message)
{
	message.getSender().send("Command not implemented yet");
	std::cout << "exec message > " << message.getCmd() << std::endl;
}
