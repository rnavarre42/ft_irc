#ifndef REPLY_HPP
# define REPLY_HPP

# include "Message.hpp"
# include <map>
# include <string>

class Reply
{
public:
	~Reply(void);
	static std::string	builder(ISender &sender, Message &message, int num, std::string p[] = 0, size_t size = 0);
	
private:
	Numeric(void);

	static Numeric			*_instance;
	Server			&server;
	Message			*message;
	int				num;
	std::string		numericStr;	

	std::string		_toString(void);

	static std::map<int, std::string>	_numericMap;
};

#endif
