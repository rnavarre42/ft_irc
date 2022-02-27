#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include "Delegate.hpp"

# include <map>
# include <iostream>

template <class KeyT, class ValueT>
class EventHandler
{
public:
	typedef typename std::multimap<KeyT, IDelegate&>	delegateMultimap_type;
	typedef typename delegateMultimap_type::iterator	delegateMultimap_iterator;

	EventHandler(void)
	{}

	~EventHandler(void)
	{}

	void	add(KeyT key, IDelegate& delegate)
	{
		this->_delegateMultimap.insert(typename std::pair<KeyT, IDelegate&>(key, delegate));
	}

	void	clear(void)
	{
		for (delegateMultimap_iterator it = this->_delegateMultimap.begin(); it != this->_delegateMultimap.end(); it++)
			delete it->second;
	}

	delegateMultimap_iterator	begin(void)
	{
		return this->_delegateMultimap.begin();
	}

	delegateMultimap_iterator	end(void)
	{
		return this->_delegateMultimap.end();
	}

	void	removeByValue(IDelegate& delegate)
	{
		delegateMultimap_type	currentIt;

		for (delegateMultimap_iterator it = this->_delegateMultimap.begin(); it != this->_delegateMultimap.end();)
		{
			currentIt = it;
			it++;
			if (currentIt->second == delegate)
			{
				this->_delegateMultimap.remove(currentIt);
				break;
			}
		}
	}

	void	raise(KeyT key, ValueT& value)
	{
		std::pair<delegateMultimap_iterator, delegateMultimap_iterator> ret;

		ret = this->_delegateMultimap.equal_range(key);
		for (delegateMultimap_iterator it = ret.first; it != ret.second; ++it)
			it->second.invoke(&value);
	}

private:
	EventHandler(const EventHandler& src);
	EventHandler&	operator=(const EventHandler& rhs);

	delegateMultimap_type	_delegateMultimap;
};

#endif
