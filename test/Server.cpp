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

	this->_eventHandler.hook(30, &joinDelegate);
	this->_eventHandler.hook(30, &partDelegate);
	this->_eventHandler.hook(40, &quitDelegate);
	this->_eventHandler.hook(30, &partDelegate);
}

void	Server::doEvents(void)
{
	this->_eventHandler.raise(30, *this);
	this->_eventHandler.raise(40, *this);
}


std::string const	&Server::getName(void) const
{
	return this->_name;
}
