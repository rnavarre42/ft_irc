#include <string.h>
#include <stddef.h>
#include "bouncer.h"

int	select_server(t_server *server)
{
	memcpy(&server->cset, &server->set, FD_SETSIZE / 8);
	server->last_fd = get_highest_fd(server->clients);
	if (server->last_fd < server->fd)
		server->last_fd = server->fd;
	server->timeout.tv_sec = server->selecttimeout;
	server->timeout.tv_usec = 0;
	return (select(server->last_fd + 2, &server->cset,
		NULL, NULL, &server->timeout));
}
