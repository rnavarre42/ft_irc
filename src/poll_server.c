#include "bouncer.h"

int	poll_server(t_server *server)
{
	server->last_fd = get_highest_fd(server->clients);
	if (server->last_fd < server->fd)
		server->last_fd = server->fd;
	server->timeout.tv_sec = server->selecttimeout;
	server->timeout.tv_usec = 0;
	return (poll(server->pollfds, MAXCLIENTS + 2, 1000)); 
}
