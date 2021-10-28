#pragma once

#include "Server.hpp"
#include "Message.hpp"
#include <string>

#define LEVEL_REGISTERED	0x01
#define LEVEL_UNREGISTERED	0x02
#define LEVEL_ALL			LEVEL_REGISTERED | LEVEL_UNREGISTERED

class Server;
class Message;

class ACommand
{
	public:
		ACommand(Server &server, char level);
		~ACommand(void);

		virtual void	exec(Message &msg) = 0;

	protected:
		Server		&server;
		char		level;
};
