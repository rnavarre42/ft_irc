#include "bouncer.h"
#include <stdlib.h>

t_command	*find_command(t_server *server, char *cmd)
{
	for (int i = 0; i < MAXCOMMANDS; i++)
	{
		if (server->commands[i].name == cmd)
			return (server->commands + i);
	}
	return (NULL);
}

t_message	*build_message(t_server *server, t_client *client, char *data)
{
	t_message	*message;

	message = malloc(sizeof(t_message));
	if (!message)
		return (NULL);
	if (has_prefix(data))
		message->mask = strtok(data + 1, " ");
	message->cmd = find_command(NULL, " ");
	if (!message->cmd)
	{
		free(message);
		return (NULL);
	}
}
