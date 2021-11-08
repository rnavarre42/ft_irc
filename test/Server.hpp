#ifndef SERVER_HPP
# define SERVER_HPP

# include "Delegate.hpp"
# include "ACommand.hpp"
# include <map>

class	ACommand;
class	Server;

class	Server
{
public:
	Server(void);

	void	start(void);
	bool	hook(int type, Delegate<ACommand> &dele);
	bool	unHook(int type, Delegate<ACommand> &dele);

private:
	void	_loadCommands(void);
	void	_raiseEvent(int type);

	std::multimap<int, Delegate>	_hookMultiMap;
//	std::map<std::string, Delegate<ACommand>>	_commandMap;

};

#endif
