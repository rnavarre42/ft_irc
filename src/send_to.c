#define __STDC_WANT_LIB_EXT2__ 1 
#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "bouncer.h"

ssize_t	send_to(t_client *client, char *data, ...)
{
	va_list ap;
	char	*strp;
	int		len;
	ssize_t	ret;

	if (!client->fd)
		return (-1);
	va_start(ap, data);
	len = vasprintf(&strp, data, ap);
	ret = send(client->fd, strp, len, 0);
	free(strp);
	return (ret);
}
