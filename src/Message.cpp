#include "Message.hpp"
#include "utils.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdio.h>

void	leftTrim(std::string &data)
{
	size_t i = 0;
	while (data[i] && data[i] == ' ')
		i++;
	data.erase(0, i);
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
	leftTrim(data);
	if (data.empty())
		return;
	if (data[0] == ':')
	{
		this->prefix = extractWord(data);
		this->prefix.erase(0, 1);
		leftTrim(data);
	}
	this->cmd = strToUpper(extractPhrase(data));
	leftTrim(data);

	while (data.size())
	{
		paramVector.push_back(extractPhrase(data));
		leftTrim(data);
	}
//	std::cout << "prefix = '" << this->prefix << "' cmd = '" << this->cmd << "' fd = " << this->sender.getFd() << std::endl;
//	for (int i = 0; i < 15 && !this->param[i].empty(); i++)
//		std::cout << "param[" << i << "] = '" << this->param[i] << "'" << std::endl;
}

std::string	Message::toString(ISender &receiver)
{
	std::ostringstream	ss;

	ss << ':' << sender.getMask() << ' ' << receiver.getName() << ' ' << cmd;
	for (size_t i = 0; i < paramVector.size(); i++)
	{
		if (paramVector[i].find(' ') == std::string::npos)
			ss << ' ';
		else
			ss << " :";
		ss <<  paramVector[i];
	}
	return ss.str();
}

Message::~Message(void)
{
//	std::cout << "Message destroyed" << std::endl;
}

std::string	&Message::operator[](size_t index)
{
	return this->paramVector[index];
}

bool Message::empty(void)
{
	return (this->prefix.empty() && this->cmd.empty() && !this->paramVector.size());
}

std::string const &Message::getCmd(void) const
{
	return this->cmd;
}

void	Message::setSender(ISender &value)
{
	this->sender = value;
}

ISender &Message::getSender(void)
{
	return this->sender;
}

size_t	Message::size(void)
{
	return this->paramVector.size();
}

Message &Message::messageBuilder(ISender &sender, std::string data)
{
	return *new Message(sender, data);
}
