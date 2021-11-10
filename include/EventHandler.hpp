#include "ACommand.hpp"
#include "Delegate.hpp"
#include <map>

class Server;

template <class T>
class EventHandler
{
public:
	typedef Delegate<ACommand, T>								delegate_type;
	typedef typename std::multimap<int, delegate_type *>::iterator			iterator_type;

	EventHandler(void);
	~EventHandler(void);

	void	hook(int type, delegate_type *dele);
	void	raise(int type, T source);

private:
	std::multimap<int, delegate_type *>	_delegateMMap;
};

template <class T>
EventHandler<T>::EventHandler(void)
{}

template <class T>
EventHandler<T>::~EventHandler(void)
{}

template <class T>
void	EventHandler<T>::hook(int type, delegate_type *dele)
{
	this->_delegateMMap.insert(std::pair<int, delegate_type *>(type, dele));
}

template <class T>
void	EventHandler<T>::raise(int type, T source)
{
	std::pair<EventHandler::iterator_type, EventHandler::iterator_type>	ret;

	ret = this->_delegateMMap.equal_range(type);
	for (EventHandler::iterator_type it = ret.first; it != ret.second; ++it)
		it->second->invoke(source);
}

