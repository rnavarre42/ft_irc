#ifndef AUSERMODE_HPP
# define AUSERMODE_HPP
// Necesitamos una clase para controlar los modos de usuario
//
// +i invisible
// en who no aparece. Names oculta su nick si no se
// comparte canal. whois muestra su modo
// 
// +o ircop
// en who aparece remarcado con una o en sus modos
// whois muestra su modo y un numeric específico
//
// AUserMode debe almacenar un flag y un modo
// 

//class User;
//class Message;

# include <cstdint>
# include <iostream>

class AUserMode
{
public:
	virtual ~AUserMode(void) {};

	enum type
	{
		canSet = 1,
		canUnset = 2
	};

	const char&	getMode(void) const
	{
		return this->_mode;
	}

	const uint64_t&	getFlag(void) const
	{
		return this->_flag;
	}

	const int&	getType(void) const
	{
		return this->_type;
	}

private:
	static uint64_t	_flagCount;

protected:
	AUserMode(const char& mode, int type)
	{
		this->_mode = mode;
		this->_type = type;
		this->_flag = AUserMode::_flagCount;
		std::cout << "flag = " << AUserMode::_flagCount << std::endl;
		std::cout << "type = " << type << std::endl;
		AUserMode::_flagCount <<= 1;
	}

	char		_mode;
	uint64_t	_flag;
	int			_type;

};

#endif
