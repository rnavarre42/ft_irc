#include "Readline.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>

Readline::Readline(void)
	: index(0)
{
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

Readline::~Readline(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

std::string &Readline::operator()(void)
{
	char		data[9];
	ssize_t		size;

	size = read(0, &data, 8);
	data[size] = '\0';

	copy.clear();
	if (*data == '\n')
	{
		copy = line;
		line.clear();
		index = 0;
		write(1, &data, 1);
	}
	else if (*data == 127)
	{
		write(1, "\033[D \033[D", 7);
		if (index)
		{
			--index;
			line.erase(index, 1);
		}
	}
	else if (!strcmp(data, "\033[C"))
	{
		if (index < line.size())
		{
			write(1, "\033[C", 3);
			++index;
		}
	}
	else if (!strcmp(data, "\033[D"))
	{
		if (index)
		{
			write(1, "\033[D", 3);
			--index;
		}
	}
	else
	{
		line.insert(index, 1, *data);
		++index;
		write(1, &data, strlen(data));
	}
	return copy;
}

