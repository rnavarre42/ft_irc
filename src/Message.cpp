#include "Message.hpp"
#include "utils.hpp"
#include "ISender.hpp"
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

Message::Message(Server &server, ISender &sender, std::string data) : _server(server), _sender(&sender), _channel(NULL), _broadcast(false)
{
	leftTrim(data);
	if (data.empty())
		return;
	if (data[0] == ':')
	{
		this->_prefix = extractWord(data);
		this->_prefix.erase(0, 1);
		leftTrim(data);
	}
	this->_cmd = strToUpper(extractPhrase(data));
	leftTrim(data);

	while (data.size())
	{
		this->_paramVector.push_back(extractPhrase(data));
		leftTrim(data);
	}
//	std::cout << "prefix = '" << this->prefix << "' cmd = '" << this->cmd << "' fd = " << this->sender.getFd() << std::endl;
//	for (int i = 0; i < 15 && !this->param[i].empty(); i++)
//		std::cout << "param[" << i << "] = '" << this->param[i] << "'" << std::endl;
}

Message::Message(Server &server, ISender &sender) : _server(server), _sender(&sender), _channel(NULL), _broadcast(false)
{}

void		Message::setReceiver(Server::userMap_type &userMap)
{
	for (Server::userMap_iterator it = userMap.begin(); it != userMap.end(); it++)
		this->_receiverVector.push_back(it->second);
}

void		Message::setReceiver(ISender *value)
{
//	this->_receiverVector.clear();
	this->_receiverVector.push_back(value);
}

ISender		*Message::getReceiver(void)
{
	return this->_receiverVector[0];
}

std::string		Message::toString(void)
{
	std::ostringstream	ss;

	ss << ':' << this->_sender->getMask();
	ss << ' ' << this->_cmd;
	if (!this->_receiverVector.empty() && !this->_broadcast)
		ss << ' ' << this->_receiverVector[0]->getName();
	for (size_t i = 0; i < this->_paramVector.size(); i++)
	{
		if (i == this->_paramVector.size() - 1)
			ss << " :";
		else
			ss << ' ';
		ss <<  this->_paramVector[i];
	}
	return ss.str();
}

Message::~Message(void)
{
//	std::cout << "Message destroyed" << std::endl;
}

std::string	&Message::operator[](size_t index)
{
	return this->_paramVector[index];
}

bool Message::empty(void)
{
	return this->_prefix.empty() && this->_cmd.empty() && !this->_paramVector.size();
}

void	Message::setBroadcast(bool value)
{
	this->_broadcast = value;
}

void	Message::limitMaxParam(size_t limit)
{
	if (limit > this->_paramVector.size())
		return;
	for (std::vector<std::string>::iterator it = this->_paramVector.begin() + limit; it != this->_paramVector.end();)
		this->_paramVector.erase(it);
}

void	Message::setCmd(std::string value)
{
	this->_cmd = value;
}

std::string const &Message::getCmd(void) const
{
	return this->_cmd;
}

void	Message::eraseAt(size_t index)
{
	std::vector<std::string>::iterator it;

	if (index > this->_paramVector.size())
		return;
	it = this->_paramVector.begin() + index;
	this->_paramVector.erase(it);
	
}
void	Message::insertField(std::string field)
{
	this->_paramVector.push_back(field);
}

void	Message::swapField(size_t first, size_t second)
{
	std::string field;

	field = this->_paramVector[first];
	this->_paramVector[first] = this->_paramVector[second];
	this->_paramVector[second] = field;
}

void	Message::setSender(ISender *value)
{
	this->_sender = value;
}

ISender *Message::getSender(void)
{
	return this->_sender;
}

Server	&Message::getServer(void)
{
	return this->_server;
}

void	Message::setChannel(Channel *value)
{
	this->_channel = value;
}

Channel	*Message::getChannel(void)
{
	return this->_channel;
}

size_t	Message::size(void)
{
	return this->_paramVector.size();
}

void	Message::send(void)
{
	for (std::vector<ISender *>::iterator it = this->_receiverVector.begin(); it != this->_receiverVector.end(); it++)
		(*it)->send(this->toString());
//	if (this->_sender->getType() == TYPE_SERVER)
//		static_cast<Server *>(this->_sender)->sendCommand(*this);
//	else if (this->_receiverVector[0]->getType() == TYPE_USER)
//		static_cast<User *>(this->_receiverVector[0])->send(*this);
}

void	Message::send(std::string msg)
{
	this->_receiverVector[0]->send(msg);
}

void	Message::process(void)
{
	static_cast<Server *>(this->_sender)->sendCommand(*this);
}

Message &Message::builder(Server &server, ISender &sender, std::string data)
{
	return *new Message(server, sender, data);
}

Message &Message::builder(Server &server, ISender &sender)
{
	return *new Message(server, sender);
}
