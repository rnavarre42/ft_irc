#include <string.h>
#include <stdlib.h>
#include "bouncer.h"

#include <stdio.h>

t_command	*find_command(t_server *server, char *cmd)
{
	for (int i = 0; i < MAXCOMMANDS; i++)
	{
		if (server->commands[i].name == cmd)
			return (server->commands + i);
	}
	return (NULL);
}

_Bool	has_prefix(char *data)
{
	return (*data == ':');
}

//	<:prefix <cmd> <arg.. 14> <:end>
t_message	*build_message(t_server *server, t_client *client, char *data)
{
	int			i;
	t_message	*message;

	(void)client;
	i = 0;
	message = malloc(sizeof(t_message));
	if (!message)
		return (NULL);
	if (has_prefix(data))
		message->mask = strsep(&data, " ");
	message->cmd = find_command(server, strsep(&data, " "));
	if (!message->cmd)
	{
		free(message);
		return (NULL);
	}
	while (data)
	{
		if (has_prefix(data))
		{
			message->argv[i] = NULL;
			message->msg = data;
			break ;
		}
		i++;
		message->argv[i] = strsep(&data, " ");
	}
	return (message);
}
