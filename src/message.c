#include "bouncer.h"
#include <stdlib.h>

t_command	*build_cmd(t_server *server, char *data)
{
	
	for (int i = 0; i < MAXCOMMANDS; i++)
	{
		if (server->commands[i] == data
	}
}
