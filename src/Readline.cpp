#include "Readline.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>

Readline::Readline(void)
	: index(0)
	, eventVector(5)
{
    tcgetattr(STDOUT_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDOUT_FILENO, TCSAFLUSH, &new_tio);

//	eventVector.push_back(std::make_pair(LF_CHR, &Readline::_newLine));
//	eventVector.push_back(std::make_pair(DEL, &Readline::_backSpace));
}

Readline::~Readline(void)
{
	tcsetattr(STDOUT_FILENO, TCSANOW, &old_tio);
	std::cout << "destructor readline called" << std::endl;
}

void Readline::_newLine(std::string const &data)
{
	std::cout << data << std::endl;
	this->copy = this->line;
	this->line.clear();
	index = 0;
}

void Readline::_backSpace(std::string const &)
{
	std::cout << "\033[D \033[D" << std::endl;
	if (index)
	{
		--index;
		line.erase(index, 1);
	}
}

std::string &Readline::operator()(void)
{
	char		data[9];
	std::string	buffer;
	ssize_t		size;

	size = read(0, &data, 8);
	data[size] = '\0';
	buffer += data;

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
		write(1, &data, strlen(data));
		line.insert(index, 1, *data);
		++index;
	}
	return copy;
}
