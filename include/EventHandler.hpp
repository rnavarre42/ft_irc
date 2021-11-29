#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include "Delegate.hpp"

# include <map>
# include <iostream>

template <class KeyT, class ValueT>
class EventHandler
{
public:
	typedef typename std::multimap<KeyT, IDelegate &>	delegateMMap_type;
	typedef typename delegateMMap_type::iterator		delegateMMap_iterator;

	EventHandler(void) {}
	~EventHandler(void) {}

	void	add(KeyT key, IDelegate &delegate)
	{
		this->_delegateMMap.insert(typename std::pair<KeyT, IDelegate &>(key, delegate));
	}

	void	clear(void)
	{
		for (delegateMMap_iterator it = this->_delegateMMap.begin(); it != this->_delegateMMap.end(); it++)
			delete it->second;
	}

	delegateMMap_iterator	begin(void)
	{
		return this->_delegateMMap.begin();
	}

	delegateMMap_iterator	end(void)
	{
		return this->_delegateMMap.end();
	}

	void	removeByValue(IDelegate &delegate)
	{
		delegateMMap_type	currentIt;

		for (delegateMMap_iterator it = this->_delegateMMap.begin(); it != this->_delegateMMap.end();)
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
		std::pair<delegateMMap_iterator, delegateMMap_iterator> ret;

		ret = this->_delegateMMap.equal_range(key);
		for (delegateMMap_iterator it = ret.first; it != ret.second; ++it)
			it->second.invoke(&value);
	}

private:
	delegateMMap_type	_delegateMMap;
};

#endif
