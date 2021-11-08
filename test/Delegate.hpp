#ifndef DELEGATE_HPP
# define DELEGATE_HPP

template <class T> //, class I>

class Delegate
{
public:
	typedef void (T::*fn)(void *arg);

	Delegate(T& target, fn operation) : _target(target), _operation(operation)
	{}

	void invoke(void *arg)
	{
		(_target.*_operation)(*arg);
	}

private:
	Delegate(void);
	Delegate(const Delegate &);

	T	&_target;
	fn	_operation;
};

#endif
