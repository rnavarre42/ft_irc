#define __STDC_WANT_LIB_EXT2__ 1 
#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "bouncer.h"

ssize_t	send_except(t_server *server, t_client *sender, char *data, ...)
{
	va_list 	ap;
	char		*strp;
	int			len;
	ssize_t		ret;
	t_client	*client;

	va_start(ap, data);
	len = vasprintf(&strp, data, ap);
	for (int i = 0; i < MAXCLIENTS; i++)
	{
		client = server->clients + i;
		if (client->fd && client != sender)
			ret = send(client->fd, strp, len, 0);
	}
	free(strp);
	return (ret);
}
