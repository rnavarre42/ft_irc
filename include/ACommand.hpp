#pragma once

#include "Server.hpp"
#include <string>

#define LEVEL_REGISTERED	0x01
#define LEVEL_UNREGISTERED	0x02
#define LEVEL_ALL			LEVEL_REGISTERED | LEVEL_UNREGISTERED

class Server;

class ACommand
{
	public:
		ACommand(Server &server, char level);
		~ACommand(void);

		virtual void	exec(void) = 0;

	protected:
		Server		&server;
		char		level;
};
