#include "Server.hpp"
#include "JoinCommand.hpp"
#include "PartCommand.hpp"
#include "QuitCommand.hpp"
#include "Delegate.hpp"
#include <string>

int main(void)
{
	JoinCommand 		join;
	PartCommand 		part;
	QuitCommand			quit;
	Server				server("irc.cojones.es");

	Delegate<CommandBase, Server &>	joinDelegate(join, &CommandBase::exec);
	Delegate<CommandBase, Server &>	partDelegate(part, &CommandBase::exec);
	Delegate<CommandBase, Server &>	quitDelegate(quit, &CommandBase::exec);
	server.hook(30, joinDelegate);
	server.hook(30, partDelegate);
	server.hook(40, quitDelegate);
	server.hook(30, partDelegate);
	server.doHook(30);
	server.doHook(40);
}
