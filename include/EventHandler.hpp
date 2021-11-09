#ifndef EVENT_HANDLER_HPP
# define EVENT_HANDLER_HPP

# include "Server.hpp"
# include <map>

class	EventHandler
{
public:
	typedef std::multimap<int, Delegate<ACommand, Server &> *>::iterator	iterator_type;

	EventHandler(void);
	~EventHandler(void);

	void	hook(int type, Delegate<ACommand, Server &> &dele);
	void	doHook(int type);

private:
	std::multimap<int, Delegate<ACommand, Server &> *>	_delegateMMap;
};

#endif
