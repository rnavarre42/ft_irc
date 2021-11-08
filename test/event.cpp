#include <map>
#include <iostream>
#include <string>
#include <functional>

#define EVENT_JOIN	0x0001
#define EVENT_PART	0x0002
#define EVENT_QUIT	0x0004


/*
 *	el puntero a método no funciona como el puntero a función. el método solo almacena el nombre del método, no de la instancia
 *	por lo que hay que proporcionar la instancia al mismo tiempo que se proporciona el puntero a método. (instancia->*puntero_a_metodo)(argumentos);
 *	De modo que la manera de almacenar 
 *
 */

class Server;
class ACommand;
typedef	void (ACommand::*TACommandFP)(Server &server);

class	PartCommand : public ACommand
{
public:
	friend class Server;
//	PartCommand(void);

	void	exec(Server &server);

private:
	void	_load(Server &server);
};

class	QuitCommand : public ACommand
{
public:
	friend class Server;
//	QuitCommand(void);

	void	exec(Server &server);
	
private:
	void	_load(Server &server);
};

class	Server
{
public:
	Server(void);

	void	start(void);
	bool	hook(int type, Delegate<ACommand &, Server &>);
	bool	unHook(int type, TACommandFP);

private:
	void	_loadCommands(void);
	void	_raiseEvent(int type);

	std::multimap<int, TACommandFP>	_hookMultiMap;
	std::map<std::string, ACommand *>	_commandMap;

};


void	JoinCommand::_load(Server &server)
{
	server.hook(EVENT_JOIN, Delegate<ACommand &, Server &>(*this, &ACommand::exec));
}

void	PartCommand::exec(Server &server)
{
	(void)server;
	std::cout << "PartEvent" << std::endl;
}

void	PartCommand::_load(Server &server)
{
	server.hook(EVENT_PART, Delegate<ACommand &, Server &>(*this, &ACommand::exec));
}

void	QuitCommand::exec(Server &server)
{
	(void)server;
	std::cout << "QuitEvent" << std::endl;
}

void	QuitCommand::_load(Server &server)
{
	server.hook(EVENT_QUIT, Delegate<ACommand &, Server &>(*this, &ACommand::exec));
}

Server::Server(void)
{
	this->_loadCommands();
}

void Server::_loadCommands(void)
{
	std::multimap<std::string, ACommand *>::iterator	it;

	this->_commandMap["JOIN"] = new JoinCommand;
	this->_commandMap["PART"] = new PartCommand;
	this->_commandMap["QUIT"] = new QuitCommand;

	for (it = this->_commandMap.begin(); it != this->_commandMap.end(); it++)
		it->second->_load(*this);
	//	std::cout << "the function '" << it->first << "' its in list" << std::endl;
}

bool	Server::hook(int type, Delegate<ACommand &, Server *> dele)
{
	(void)type;
	(void)dele;
/*	std::pair<std::multimap<int, TACommandFP>::iterator, std::multimap<int, TACommandFP>::iterator> ret;
	std::multimap<int, TACommandFP>::iterator	it;

	ret = this->_hookMultiMap.equal_range(type);
	for (std::multimap<int, TACommandFP>::iterator it = ret.first; it != ret.second; it++)
	{
		if (it->second == func)
			return false;
	}
	(_commandMap["JOIN"]->*func)(*this);
	this->_hookMultiMap.insert(std::pair<int, TACommandFP>(type, func));
*/	return true;
}

bool	Server::unHook(int type, TACommandFP func)
{
	std::pair<std::multimap<int, TACommandFP>::iterator, std::multimap<int, TACommandFP>::iterator> ret;
	std::multimap<int, TACommandFP>::iterator	it;

	ret = this->_hookMultiMap.equal_range(type);
	for (std::multimap<int, TACommandFP>::iterator it = ret.first; it != ret.second; it++)
	{
		if (it->second == func)
		{
			this->_hookMultiMap.erase(it);
			return true;
		}
	}
	return false;
}

void	Server::_raiseEvent(int type)
{
	std::pair<std::multimap<int, TACommandFP>::iterator, std::multimap<int, TACommandFP>::iterator> ret;
	std::multimap<int, TACommandFP>::iterator it;

	ret = this->_hookMultiMap.equal_range(type);
	//for (std::multimap<int, void (ACommand::*)(Server &server)>::iterator it = ret.first; it != ret.second; it++)
	for (it = ret.first; it != ret.second; it++)
	{
		//TACommandFP fp = it->second;
//		(this->_commandMap["JOIN"]->*fp)(*this);
		std::cout << std::hex << it->second << std::endl;
		(this->_commandMap["JOIN"]->*it->second)(*this);
//		(*it).second.*(*this);
//		dada = it->second;
//		reinterpret_cast<void (ACommand::*)(Server &server)>((*it).second)(*this)*;
//		std::cout << std::hex << it->*second << std::endl;
	}
}

void	Server::start(void)
{
	this->_raiseEvent(EVENT_JOIN);
	this->_raiseEvent(EVENT_PART);
	this->_raiseEvent(EVENT_QUIT);
}

int	main(void)
{
	Server	server;

	server.start();
/*	if (!server.hook(EVENT_JOIN, JoinEvent))
		std::cout << "Error, event / function already exists" << std::endl;
	if (!server.hook(EVENT_JOIN, JoinEvent))
		std::cout << "Error, event / function already exists" << std::endl;
	if (!server.hook(EVENT_JOIN, JoinEvent2))
		std::cout << "Error, event / function already exists" << std::endl;
	if (!server.hook(EVENT_PART, PartEvent))
		std::cout << "Error, event / function already exists" << std::endl;
	if (!server.hook(EVENT_QUIT, QuitEvent))
		std::cout << "Error, event / function already exists" << std::endl;
	if (!server.unHook(EVENT_JOIN, JoinEvent2))
		std::cout << "Error, event / function not exists" << std::endl;
*/	return 0;
}
