#include <map>
#include <string>

class Chan
{
public:
	Chan(void) {}


};

int main(void)
{
	std::map<std::string, void *> test;

	test["a"]= new Chan();
}
