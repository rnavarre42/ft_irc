#ifndef SERVER_HPP
# define SERVER_HPP

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

	void	start(void);
	void	doEvents(void);
	void	hook(int type, DelegateBase &dele);
	void	unHook(int type, DelegateBase &dele);
	void	doHook(int type);

	std::string	const &getName(void) const;
private:
	void	_loadCommands(void);
	void	_raiseEvent(int type);

	const std::string		&_name;
	std::multimap<int, DelegateBase	*> _delegateMMap;

};

#endif
