#ifndef DELEGATEBASE_HPP
# define DELEGATEBASE_HPP

class DelegateBase
{
	virtual void invoke(void *arg) = 0;
};

#endif
