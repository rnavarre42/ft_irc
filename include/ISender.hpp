#pragma once

#include <string>
#include <sys/types.h>

class ISender
{

public:
	virtual ~ISender(void);

	virtual std::string const	&getName(void) const = 0;
	virtual int const			&getFd(void) const = 0;
	virtual ssize_t				send(std::string msg) = 0;
	virtual bool const			&isRegistered(void) const = 0;

};
