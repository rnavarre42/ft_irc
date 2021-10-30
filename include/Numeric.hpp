#ifndef NUMERIC_HPP
# define NUMERIC_HPP

# include "Message.hpp"
# include <map>
# include <string>

class Numeric
{
public:
	~Numeric(void);
	static std::string	builder(Server &server, Message &message, int num, std::string p[] = 0, size_t size = 0);
	
private:
	Numeric(Server &server);

	static Numeric			*_instance;
	Server			&server;
	Message			*message;
	int				num;
	std::string		numericStr;	

	std::string		_toString(void);

	static std::map<int, std::string>	_numericMap;
};

#endif
