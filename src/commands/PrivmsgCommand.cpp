#include "PrivmsgCommand.hpp"
#include "Message.hpp"
#include "Server.hpp"
#include <iostream>

PrivmsgCommand::PrivmsgCommand(Server &server, char level) : ACommand(server, level)
{}

void PrivmsgCommand::_exec(Message &message)
{
	message.getSender().send("Command not implemented yet");
	std::cout << "exec message > " << message.getCmd() << std::endl;
}
