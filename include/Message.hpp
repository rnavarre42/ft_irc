#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Server.hpp"
# include "ACommand.hpp"
# include <string>
# include <vector>

class Message
{
public:
	~Message(void);

	std::string			&operator[](size_t index);
	std::string const	&getCmd(void) const;

	void				setSender(ISender &value);
	ISender				&getSender(void);

	size_t				size(void);
	bool				empty(void);

	std::string			toString(ISender &receiver);
	static Message		&messageBuilder(ISender &sender, std::string data);

private:
	Message(ISender &sender, std::string data);

	ISender		&sender;
//	std::string	name;
//	std::string	ident;
//	std::string	host;
	std::string prefix;
	std::string cmd;
	std::vector<std::string>	paramVector;
};

#endif
