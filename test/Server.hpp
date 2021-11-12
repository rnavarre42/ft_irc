#ifndef SERVER_HPP
# define SERVER_HPP

# include "EventHandler.hpp"
# include "DelegateBase.hpp"
# include "CommandBase.hpp"
# include <string>
# include <map>

class	CommandBase;

class	Server
{
public:
	typedef	std::multimap<int, DelegateBase *>::iterator	iterator_type;

	Server(const std::string &name);

	void	doEvents(void);

	std::string	const &getName(void) const;
private:
	void	_loadCommands(void);
	void	_raiseEvent(int type);

	const std::string		&_name;
	EventHandler<Server &>			_eventHandler;
};

#endif
