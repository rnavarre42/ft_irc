#ifndef DELEGATE_HPP
# define DELEGATE_HPP

class DelegateBase
{
	virtual void invoke(void *arg) = 0;
};

template <class T, class EventT> //, class I>
class Delegate : public DelegateBase
{
public:
	typedef void (T::*fn)(EventT);

	Delegate(T& target, fn operation) : _target(target), _operation(operation)
	{}

	void invoke(void *arg)
	{
		(_target.*_operation)(static_cast<EventT>(arg));
	}

private:
//	Delegate(void);
//	Delegate(const Delegate &);

	T	&_target;
	fn	_operation;
};

#endif
