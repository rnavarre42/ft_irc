#include <unistd.h>
#include "bouncer.h"

t_client	*get_client_slot(t_server *server)
{
	for (int i = 0; i < MAXCLIENTS; i++)
	{
		if (!server->clients[i].fd)
			return (server->clients + i);
	}
	return (NULL);
}
