#include "irc_bouncer.h"

void	read_clients(t_server *ircserver)
{
	int			size;
	char		buffer[BUFFERSIZE] = {0};
	t_client	*client;

	for (int i = 0; i < MAXCLIENTS; i++)
	{
		client = ircserver->clients + i;
		if (client->fd && FD_ISSET(client->fd, &ircserver->cset))
		{
			size = read(client->fd, buffer, BUFFERSIZE);
			if (size == -1 || telnet_ctrlc(buffer))
			{
				printf("Client #%lu close connection\n", client->id);
				FD_CLR(client->fd, &ircserver->set);
				close(client->fd);
				client->fd = 0;
				ircserver->clients_count--;
			}
			else
			{
				buffer[size] = 0;
				if (size != 0)
					printf("#%lu> %s", client->id, buffer);
				send_to(client, "server> ");
			}
		}
	}
}
