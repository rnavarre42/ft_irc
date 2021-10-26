#pragma once

#include "Server.hpp"
#include <string>

#define LEVEL_REGISTERED	0x01
#define LEVEL_UNREGISTERED	0x02
#define LEVEL_ALL			LEVEL_REGISTERED | LEVEL_UNREGISTERED

class CommandBase
{
	public:
		CommandBase(Server &server, std::string name, char level);
		~CommandBase(void);

		virtual void	exec(void) = 0;

	protected:
		Server		&server;
		std::string	name;
		char		level;
};

class UserCommand : CommandBase
{
	public:
		UserCommand(Server &server, char level);

		virtual void	exec(void);
};
