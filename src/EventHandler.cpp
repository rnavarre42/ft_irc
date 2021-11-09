#include "EventHandler.hpp"

EventHandler::EventHandler(void)
{}

EventHandler::~EventHandler(void)
{}

void	EventHandler::hook(int type, delegate_type *dele)
{
	this->_delegateMMap.insert(std::pair<int, delegate_type *>(type, dele));
}

void	EventHandler::doHook(int type, T source)
{
	std::pair<iterator_type, iterator_type>	ret;

	ret = this->_delegateMMap.equal_range(type);
	for (iterator_type it = ret.first; it != ret.second; ++it)
		it->second->invoke(source);
}
