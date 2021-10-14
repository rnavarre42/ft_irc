#include <stdlib.h>
#include "bouncer.h"

t_server	*singleton_server(void)
{
	static t_server	*server;

	if (!server)
		server = malloc(sizeof(t_server));
	return (server);
}
