#include "Server.hpp"
#include "Delegate.hpp"
#include "JoinCommand.hpp"
#include "PartCommand.hpp"
#include "QuitCommand.hpp"

Server::Server(const std::string &name) : _name(name)
{
	JoinCommand 		join;
	PartCommand 		part;
	QuitCommand			quit;

	Delegate<CommandBase, Server &>	joinDelegate(join, &CommandBase::exec);
	Delegate<CommandBase, Server &>	partDelegate(part, &CommandBase::exec);
	Delegate<CommandBase, Server &>	quitDelegate(quit, &CommandBase::exec);

	this->hook(30, joinDelegate);
	this->hook(30, partDelegate);
	this->hook(40, quitDelegate);
	this->hook(30, partDelegate);
}

void	Server::doEvents(void)
{
	this->doHook(30);
	this->doHook(40);
}


std::string const	&Server::getName(void) const
{
	return this->_name;
}
