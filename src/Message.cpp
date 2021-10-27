#include "Message.hpp"
#include <string>
#include <iostream>
#include <stdio.h>

std::string	substrchr(std::string str, size_t *offset, char chr)
{
	size_t pos;
	size_t startOffset;


	if (str[*offset] == ':')
	{
		startOffset = *offset + 1;
		*offset = str.size();
		return str.substr(startOffset, str.size() - startOffset);
	}
	while (str[*offset] == chr)
		(*offset)++;
	pos = str.find(chr, *offset);
	if (pos != std::string::npos)
	{
		startOffset = *offset;
		*offset += pos;
		return str.substr(startOffset, pos - startOffset);
	}
	return str.substr(*offset, str.size() - *offset);
}

Message::Message(ISender &sender, std::string data) : sender(sender)
{
	size_t	offset;
//	servidor> :irc.chathispano.org eriuwer JOIN #canal
//	cliente> :origen comando parametos :erir

	//verifica si tiene prefijo
	offset = 0;
	while (data[offset] == ' ')
		offset++;
	if (data.find(':', offset) != std::string::npos)
	{
//		offset = 1;
		this->prefix = data.substr(offset, data.find(' ', offset) - offset);
		offset += this->prefix.size();
//		this->prefix = substrchr(data, &offset, ' ');
	}
	this->cmd = substrchr(data, &offset, ' ');
	std::cout << "prefix = '" << this->prefix << "' cmd = '" << this->cmd << "' fd = " << this->sender.getFd() << std::endl;
}

Message::~Message(void)
{
	std::cout << "Message destroyed" << std::endl;
}

Message &Message::messageBuilder(ISender &sender, std::string data)
{
	return(*new Message(sender, data));
}

std::string	const *Message::getParam(int index) const
{
	if (index <= this->size)
		return (this->param + index);
	return NULL;
}
