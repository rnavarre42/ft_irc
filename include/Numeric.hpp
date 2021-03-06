#ifndef NUMERIC_HPP
# define NUMERIC_HPP

# include "numerics.hpp"
# include "Message.hpp"

# include <map>
# include <vector>
# include <string>

class Server;

class Numeric
{
public:
	typedef std::vector<std::string>	stringVector_type;
	typedef stringVector_type::iterator	stringVector_iterator;

	~Numeric(void);

	static std::string	builder(Server& server, ISender& sender, int num);
	static void			insertField(size_t number);
	static void			insertField(std::string data);
	static void			insertField(char chr);
	static void			clear(void);
	
	static std::string	builder(Message& message, int num)
	{
		return Numeric::builder(*message.getServer(), *message.getSender(), num);
	}

private:
	static Numeric*						_instance;
	static std::map<int, std::string>	_numericMap;

	static void		_initInstance(Server& server, ISender& sender, int num);
	static bool		_buildMulti(std::string& data, std::string& base,
							size_t offset, stringVector_iterator& initialPos);
	
	int							_num;
	Server*						_server;
	ISender*					_sender;
	std::string					_numericStr;	
	std::vector<std::string>	_fieldVector;

	std::string	_toString(void);

	Numeric(void);
	Numeric(const Numeric&src);
	Numeric&	operator=(const Numeric& rhs);
};

#endif
