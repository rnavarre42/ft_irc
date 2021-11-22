#ifndef NUMERIC_HPP
# define NUMERIC_HPP

# include "numerics.hpp"
# include "Message.hpp"
# include "Source.hpp"
# include "Server.hpp"
# include <map>
# include <vector>
# include <string>

class Numeric
{
public:
	~Numeric(void);
	static std::string			builder(Server &server, ISender &sender, int num);
	static void					insertField(std::string data);
	
	inline static std::string	builder(Source &source, int num)
	{
		return Numeric::builder(*source.server, *source.message->getSender(), num);
	}

private:
	Numeric(void);

	static Numeric						*_instance;
	static std::map<int, std::string>	_numericMap;
	
	int									_num;
	Server								*_server;
	ISender								*_sender;
	std::string							_numericStr;	
	std::vector<std::string>			_fieldVector;

	std::string							_toString(void);
};

#endif
