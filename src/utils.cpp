#include <string>

std::string strToUpper(std::string data)
{
	for (size_t i = 0; i < data.size(); i++)
		if (data[i] >= 'a' && data[i] <= 'z')
			data[i] -= 32;
	return data;
}
