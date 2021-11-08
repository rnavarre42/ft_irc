#include "Server.hpp"
#include "JoinCommand.hpp"

Server::Server(void)
{}

void	Server::start(void)
{
}

bool	Server::hook(int type, Delegate<ACommand> &dele)
{
	(void)type;
//	(void)dele;
	//(&JoinCommand::exec, *this)(type);

	this->_hookMultiMap[type] = dele;
//	this->_hookMultiMap.insert(std::pair<int, Delegate(dele)>(type));
	return true;
}

bool	Server::unHook(int type, Delegate<ACommand> &dele)
{
	(void)type;
	(void)dele;
	return true;
}
