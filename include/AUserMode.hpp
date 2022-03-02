#ifndef AUSERMODE_HPP
# define AUSERMODE_HPP

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
		AUserMode::_flagCount <<= 1;
	}

	char		_mode;
	uint64_t	_flag;
	int			_type;

};

#endif
