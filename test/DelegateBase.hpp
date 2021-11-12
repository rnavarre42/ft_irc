#ifndef DELEGATEBASE_HPP
# define DELEGATEBASE_HPP

class DelegateBase
{
public:
	virtual void invoke(void *arg) = 0;
};

#endif
