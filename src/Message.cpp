#include "Message.hpp"
#include <iostream>

Message::Message(ISender &sender, std::string data) : sender(sender)
{
	if (data.find(':', 0) == 0)
		std::cout << "encontrado en cero" << this->sender.getName() << std::endl;
	else
		std::cout << "No encontrado en cero" << std::endl;
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
		return (this->param[index]);
	return NULL;
}
