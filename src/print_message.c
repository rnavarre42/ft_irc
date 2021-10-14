#include <stdio.h>
#include "bouncer.h"

void	print_message(t_message *message)
{
	int	i;

	i = 0;
	if (!message)
		return ;
	if (message->mask)
		printf("message->mask = '%s'\n", message->mask);
	printf("message->source->id = %lu\n", message->source->id);
	printf("message->target->id = %lu\n", message->target->id);
	printf("message->cmd->name = '%s'ºn", message->cmd->name);
	while (message->argv[i])
	{
		printf("message->arg[%i] = '%s'\n", i, message->argv[i]);
		i++;
	}
	printf("message->msg = '%s'\n", message->msg);
	printf("\n");
}
