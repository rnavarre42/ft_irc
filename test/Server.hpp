#ifndef SERVER_HPP
# define SERVER_HPP

# include "Delegate.hpp"
# include "CommandBase.hpp"
# include <string>
# include <map>

class	CommandBase;

class	Server
{
public:
	typedef	std::multimap<int, Delegate<CommandBase, Server &> *>::iterator	iterator_type;

	Server(const std::string &name) : _name(name)
	{}

	void	start(void);
	void	hook(int type, Delegate<CommandBase, Server &> &dele);
	void	unHook(int type, Delegate<CommandBase, Server &> &dele);
	void	doHook(int type);

	std::string	const &getName(void) const;
private:
	void	_loadCommands(void);
	void	_raiseEvent(int type);

	const std::string		&_name;
	std::multimap<int, Delegate<CommandBase, Server &>*>	_delegateMMap;

};

#endif
