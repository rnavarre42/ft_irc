#include <unistd.h>
#include <stdio.h>
#include "bouncer.h"

void	read_clients(t_server *server)
{
	int			size;
	char		buffer[BUFFERSIZE] = {0};
	t_client	*client;
//	t_message	*message;

	for (int i = 0; i < MAXCLIENTS; i++)
	{
		client = server->clients + i;
		if (server->pollfds[i + 2].revents & POLLIN)
		{
			size = read(client->fd, buffer, BUFFERSIZE);
			if (size == 0 || telnet_ctrlc(buffer))
				disconnect_client(server, client);
			else
			{
				buffer[size] = 0;
				if (size != 0)
				{
					//message = build_message(server, client, buffer);
					//print_message(message);
					printf("#%lu> %s", client->id, buffer);
					send_except(server, client, "#%lu> %s\r", client->id, buffer);
				}
			}
		}
	}
}
