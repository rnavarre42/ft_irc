#include "ISender.hpp"

class Unknown : public ISender
{
public:
	std::string const	&getName(void) const;
	int const			&getFd(void) const;
	std::string const	&getPass(void) const;
};
