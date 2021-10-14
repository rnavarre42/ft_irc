#include <unistd.h>
#include <stdio.h>
#include "bouncer.h"

void	read_clients(t_server *server)
{
	int			size;
	char		buffer[BUFFERSIZE] = {0};
	t_client	*client;

	for (int i = 0; i < MAXCLIENTS; i++)
	{
		client = server->clients + i;
		if (client->fd && FD_ISSET(client->fd, &server->cset))
		{
			size = read(client->fd, buffer, BUFFERSIZE);
			if (size == -1 || telnet_ctrlc(buffer))
				disconnect_client(server, client);
			else
			{
				buffer[size] = 0;
				if (size != 0)
				{
					printf("#%lu> %s", client->id, buffer);
					send_except(server, client, "#%lu> %s\r", client->id, buffer);
				}
			}
		}
	}
}
