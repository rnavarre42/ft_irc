#include "bouncer.h"
#include <stdio.h>

int	get_highest_fd(t_client *clients)
{
	int	highest_fd;

	highest_fd = clients[0].fd;
	for (int i = 1; i < MAXCLIENTS; i++)
	{
//		printf("client.fd = %d\n", clients[i].fd);
		if (highest_fd < clients[i].fd)
			highest_fd = clients[i].fd;
	}
	return (highest_fd);
}
