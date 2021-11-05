#ifndef MESSAGE_HPP
# define MESSAGE_HPP

//# include "Server.hpp"
# include "ACommand.hpp"
//# include "ISender.hpp"
# include <string>
# include <vector>

class ISender;
class User;
class Server;

class Message
{
public:
	~Message(void);

	std::string			&operator[](size_t index);

	void				setCmd(std::string value);
	std::string const	&getCmd(void) const;

	void				setReceiver(ISender *value);
	ISender				*getReceiver(void);

	void				setSender(ISender &value);
	ISender				&getSender(void);

	size_t				size(void);
	bool				empty(void);
	void				eraseAt(size_t index);

	void				limitMaxParam(size_t limit);

	void				swapField(size_t first, size_t second);
	void				insertField(std::string field);
	std::string			toString(void);
	static Message		&messageBuilder(ISender &sender, std::string data);
	static Message		&messageBuilder(ISender &sender);

private:
	Message(ISender &sender, std::string data);
	Message(ISender &sender);

	ISender		*sender;
	ISender		*receiver;
//	std::string	name;
//	std::string	ident;
//	std::string	host;
	std::string prefix;
	std::string cmd;
	std::vector<std::string>	paramVector;
};

#endif
