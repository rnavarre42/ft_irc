#include "bouncer.h"

int	get_poll_slot(t_server *server)
{
	for (int i = 0; i < MAXCLIENTS; i++)
	{
		if (!server->pollfds[i + 2].fd)
			return (i + 2);
	}
	return (0);
}
