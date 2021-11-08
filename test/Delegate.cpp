#include "Delegate.hpp"
#include <iostream>
#include <string>

class A
{
public:
	void Fn(std::string &i)
	{
		std::cout << " i = " << i << std::endl;
	}
};


int main(void)
{
	A a;
	std::string	str = "Pollax";

	Delegate<A, std::string &>	dele(a, &A::Fn);

	dele.execute(str);
}
