#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

//# include "ACommand.hpp"
# include "Delegate.hpp"
# include <map>
# include <iostream>

class Server;
class ACommand;
class IDelegate;

template <class KeyT, class ValueT>
class EventHandler
{
public:
	typedef typename std::multimap<KeyT, IDelegate &>	multimap_type;
	typedef typename multimap_type::iterator			iterator_type;

	EventHandler(void) {}
	~EventHandler(void) {}

	void	add(KeyT key, IDelegate &delegate)
	{
		this->_delegateMMap.insert(typename std::pair<KeyT, IDelegate &>(key, delegate));
	}

	void	removeByValue(IDelegate &delegate)
	{
		iterator_type	currentIt;

		for (iterator_type it = this->_delegateMMap.begin(); it != this->_delegateMMap.end();)
		{
			currentIt = it;
			it++;
			if (currentIt->second == delegate)
			{
				this->_delegateMMap.remove(currentIt);
				break;
			}
		}
	}

	void	raise(KeyT key, ValueT &value)
	{
		std::pair<iterator_type, iterator_type> ret;

		ret = this->_delegateMMap.equal_range(key);
		for (iterator_type it = ret.first; it != ret.second; ++it)
			it->second.invoke(&value);
	}

private:
	multimap_type	_delegateMMap;
};

#endif
