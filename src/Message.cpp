#include "Message.hpp"
#include "User.hpp"
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

Message::Message(Server &server) : _server(server), _sender(NULL), _channel(NULL), _command(NULL), _hideReceiver(false)
{}

void Message::set(ISender &sender, std::string data)
{
	this->_sender = &sender;
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

//Message::Message(ISender &sender) : _sender(&sender), _channel(NULL), _broadcast(false)
//{}

void		Message::setReceiver(std::map<std::string, std::pair<int, User *> > &userMap)
{
	for (std::map<std::string, std::pair<int, User *> >::iterator it = userMap.begin(); it != userMap.end(); it++)
		if (it->second.second != this->_sender)
			this->_receiverVector.push_back(it->second.second);
}

void		Message::setReceiver(Server::userVector_type &userVector)
{
	for (Server::userVector_iterator it = userVector.begin(); it != userVector.end(); it++)
		if (*it != this->_sender)
			this->_receiverVector.push_back(*it);
}

void		Message::setReceiver(ISender *value)
{
//	this->_receiverVector.clear();
	this->_receiverVector.push_back(value);
}

ISender		*Message::getReceiver(void)
{
	if (this->_receiverVector.size())
		return this->_receiverVector[0];
	return NULL;
}

std::string		Message::toString(void)
{
	std::ostringstream	ss;

	ss << ':' << this->_sender->getMask();
	ss << ' ' << this->_cmd;
	if (!this->_receiverVector.empty() && !this->_hideReceiver)
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
{}

std::string	&Message::operator[](size_t index)
{
	return this->_paramVector[index];
}

bool Message::empty(void)
{
	return this->_prefix.empty() && this->_cmd.empty() && !this->_paramVector.size();
}

void	Message::hideReceiver(void)
{
	this->_hideReceiver = true;
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
//	this->_server.commandF(
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

Server *Message::getServer(void)
{
	return &this->_server;
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

void	Message::clear(void)
{
	this->_receiverVector.clear();
	this->_sender = NULL;
	this->_paramVector.clear();
	this->_cmd.clear();
	this->_channel = NULL;
	this->_hideReceiver = false;
}

void	Message::clearReceiver(void)
{
	this->_receiverVector.clear();
}

void	Message::reply(void)
{
	this->_sender->send(this->toString());
}

void	Message::reply(std::string data)
{
	this->_sender->send(data);
}

void	Message::send(void)
{
	std::string data = this->toString();

	if (this->_receiverVector.size() > 1)
		this->_hideReceiver = true;
	for (std::vector<ISender *>::iterator it = this->_receiverVector.begin(); it != this->_receiverVector.end(); it++)
			(*it)->send(data);
}

void	Message::send(std::string data)
{
	this->_receiverVector[0]->send(data);
}

void	Message::process(void)
{
	this->_server.sendCommand(*this);
}
