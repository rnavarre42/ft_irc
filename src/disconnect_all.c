#include <unistd.h>
#include "bouncer.h"

void	disconnect_all(t_server *server, char *msg)
{
	t_client	*client;

	for (int i = 0; i < MAXCLIENTS; i++)
	{
		client = server->clients + i;
		if (send_to(client, "disconnected: %s\n\r", msg) > 0)
			disconnect_client(server, client);
	}
}
