#include <map>
#include <string>
#include <iostream>
#include <sstream>

class Numeric
{
public:
	static void Builder(void);

private:
	Numeric(void);
	std::string		_toString(void);

	static Numeric	*_instance;
	std::string		_tio = "Tio esto es la leche";
};

Numeric	*Numeric::_instance;

void	Numeric::Builder(void)
{
	if (!_instance)
		_instance = new Numeric();
	std::cout << _toString() << std::endl;
}
	
Numeric::Numeric(void)
{}

std::string		Numeric::_toString(void)
{
	return "tio";
	//return this->_tio;
}

int main(void)
{
	Numeric::Builder();

	return 0;
}
