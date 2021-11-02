#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Server.hpp"
# include "ACommand.hpp"
# include <string>

class Message
{
public:
	~Message(void);

	std::string const	*getParam(size_t index) const;
	std::string const	&getCmd(void) const;
	ISender				&getSender(void);
	size_t				size(void);
	bool				empty(void);
	static Message		&messageBuilder(ISender &sender, std::string data);

private:
	Message(ISender &sender, std::string data);

	ISender		&sender;
//	std::string	name;
//	std::string	ident;
//	std::string	host;
	std::string prefix;
	std::string cmd;
	size_t		_size;
	std::string	param[15];
};

#endif
