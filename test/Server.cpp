#include "Server.hpp"

std::string const	&Server::getName(void) const
{
	return this->_name;
}

void	Server::hook(int type, Delegate<CommandBase, Server &> &dele)
{
	//std::string	str = "pofale";

	_delegateMMap.insert(std::pair<int, Delegate<CommandBase, Server &>*>(type, &dele));
	//delegateMMap[type] = &dele;
	//std::cout << "type = " << type << ": dele.invoke --";
	//dele.invoke(str);
}

void	Server::doHook(int type)
{
	std::string	str = "Server invoke";
	std::pair<std::multimap<int, Delegate<CommandBase, Server &>*>::iterator, std::multimap<int, Delegate<CommandBase, Server &>*>::iterator> ret;

	//std::multimap<int, Delegate<CommandBase, std::string &>*>::iterator	it;
	//it = delegateMMap.find(type);
	
	ret = _delegateMMap.equal_range(type);
	for (std::multimap<int, Delegate<CommandBase, Server &>*>::iterator it=ret.first; it != ret.second; ++it)
	{
		it->second->invoke(*this);
	}

	//if (it != delegateMap.end())
	//	it->second->invoke(str);
}
