#include "EventHandler.hpp"

EventHandler::EventHandler(void)
{}

EventHandler::~EventHandler(void)
{}

void	EventHandler::hook(int type, Delegate<ACommand, Server &> &dele)
{
	this->_delegateMMap.insert(std::pair<int, Delegate<ACommand, Server &> *>(type, &dele));
}

void	EventHandler::doHook(int type)
{
	std::pair<iterator_type, iterator_type>	ret;

	ret = this->_delegateMMap.equal_range(type);
	for (iterator_type it = ret.first; it != ret.second; ++it)
		it->second->invoke();
}
