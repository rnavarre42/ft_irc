#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Server.hpp"
# include "CommandBase.hpp"
# include <string>

class Message
{
public:
	~Message(void);

	std::string const	*getParam(int index) const;
	static Message		&messageBuilder(ISender &sender, std::string data);

private:
	Message(ISender &sender, std::string data);

	ISender		&sender;
	std::string	name;
	std::string	ident;
	std::string	host;
	std::string prefix;
	std::string cmd;
	int			size;
	std::string	param[15];
};

#endif
