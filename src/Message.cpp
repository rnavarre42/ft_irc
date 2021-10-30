#include "Message.hpp"
#include <string>
#include <iostream>
#include <stdio.h>

void	leftTrim(std::string &data)
{
	size_t i = 0;
	while (data[i] && data[i] == ' ')
		i++;
	data.erase(0, i);
}

void strToUpper(std::string &data)
{
	for (size_t i = 0; i < data.size(); i++)
		if (data[i] >= 'a' && data[i] <= 'z')
			data[i] -= 32;
}

std::string	extractWord(std::string &data)
{
	size_t 		pos;
	std::string	newStr;

	pos = data.find(' ');
	if (pos != std::string::npos)
	{
		newStr = data.substr(0, pos);
		data.erase(0, pos);
		return newStr;
	}
	newStr = data;
	data.clear();
	return newStr;
}

std::string extractPhrase(std::string &data)
{
	std::string newStr;

	if (data[0] == ':')
	{
		newStr = data;
		newStr.erase(0, 1);
		data.clear();
		return newStr;
	}
	else
		return extractWord(data);
}

Message::Message(ISender &sender, std::string data) : sender(sender)
{
	int i = 0;
//	std::cout << "message original: '" << data << "'" << std::endl;
	leftTrim(data);
	if (data[0] == ':')
	{
		this->prefix = extractWord(data);
		this->prefix.erase(0, 1);
		leftTrim(data);
	}
	this->cmd = extractPhrase(data);
	strToUpper(this->cmd);
	leftTrim(data);
	for (; i < 15 && data.size() > 1; i++)
	{
		this->param[i] = extractPhrase(data);
		leftTrim(data);
	}
	this->count = i + 1;
//	std::cout << "prefix = '" << this->prefix << "' cmd = '" << this->cmd << "' fd = " << this->sender.getFd() << std::endl;
//	for (int i = 0; i < 15 && !this->param[i].empty(); i++)
//		std::cout << "param[" << i << "] = '" << this->param[i] << "'" << std::endl;
}

Message::~Message(void)
{
//	std::cout << "Message destroyed" << std::endl;
}

std::string const &Message::getCmd(void) const
{
	return this->cmd;
}

ISender &Message::getSender(void)
{
	return this->sender;
}

int		Message::getCount(void)
{
	return this->count;
}

Message &Message::messageBuilder(ISender &sender, std::string data)
{
	return *new Message(sender, data);
}

std::string	const *Message::getParam(int index) const
{
	if (index <= this->count)
		return this->param + index;
	return NULL;
}
