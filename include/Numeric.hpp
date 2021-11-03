#ifndef NUMERIC_HPP
# define NUMERIC_HPP

# include "numerics.hpp"
# include "Message.hpp"
# include <map>
# include <vector>
# include <string>

class Numeric
{
public:
	~Numeric(void);
	static std::string	builder(Server &server, ISender &sender, int num);
	static void			insertField(std::string data);
	
private:
	Numeric(void);

	static Numeric			*_instance;
	static std::map<int, std::string>	_numericMap;
	
	int							num;
	Server						*server;
	ISender						*sender;
	std::string					numericStr;	
	std::vector<std::string>	fieldVector;

	std::string					_toString(void);
};

#endif
