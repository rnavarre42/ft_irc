#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include "ACommand.hpp"
# include "Delegate.hpp"
# include <map>

class	ACommand;

template<class T>
class	EventHandler
{
public:
	typedef Delegate<ACommand, T &>								delegate_type;
	typename std::multimap<int, Delegate<ACommand, T &> *>::iterator			iterator_type;

	EventHandler(void);
	~EventHandler(void);

	void	hook(int type, delegate_type *dele);
	void	doHook(int type, T source);

private:
	std::multimap<int, delegate_type *>	_delegateMMap;
};

#endif

