#include "CommandBase.hpp"
#include "Delegate.hpp"
#include <map>
#include <typeinfo>
#include <iostream>

class Server;

template <class T>
class EventHandler
{
public:
	typedef DelegateBase	delegate_type;
	typedef std::multimap<int, delegate_type *>::iterator	iterator_type;

	EventHandler(void);
	~EventHandler(void);

	void	hook(int type, void *dele);
	void	raise(int type, T source);

private:
	std::multimap<int, delegate_type *> _delegateMMap;
};

template <class T>
EventHandler<T>::EventHandler(void)
{}

template <class T>
EventHandler<T>::~EventHandler(void)
{}

template <class T>
void	EventHandler<T>::hook(int type, void *dele)
{
	std::cout << "EventHandler type.type = " << typeid(*dele).name() << std::endl; 
	this->_delegateMMap.insert(std::pair<int, delegate_type *>(type, reinterpret_cast<delegate_type *>(dele)));
}

template <class T>
void	EventHandler<T>::raise(int type, T source)
{
	std::pair<EventHandler::iterator_type, EventHandler::iterator_type> ret;

	ret = this->_delegateMMap.equal_range(type);
	for (EventHandler::iterator_type it = ret.first; it != ret.second; ++it)
	{
		std::cout << typeid(it->second).name() << std::endl;
		it->second->invoke(&source);
	}
}
