#include <stdio.h>
#include <string.h>
#include "bouncer.h"

_Bool	telnet_ctrlc(char *buffer)
{
	char exit[] = {255, 244, 255, 253, 6};

	return (!strncmp(buffer, exit, 5));
}

int main(void)
{
	printf("Waiting connect clients...\n");
	start_server("0.0.0.0", 6667, SELECTTIMEOUT);
}
