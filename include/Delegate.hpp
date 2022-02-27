#ifndef DELEGATE_HPP
# define DELEGATE_HPP

class IDelegate
{
public:
	virtual ~IDelegate(void)
	{}

	virtual void	invoke(void* data) = 0;
};

template <class KeyT, class ValueT>
class Delegate : public IDelegate
{
public:
	typedef void (KeyT::*fn)(ValueT&);

	Delegate(KeyT& target, fn prototype)
		: _target(target), _prototype(prototype)
	{}

	~Delegate(void)
	{}

	void invoke(void* data)
	{
		(_target.*_prototype)(*(static_cast<ValueT*>(data)));
	}

private:
	Delegate(void);
	Delegate(const Delegate& src);
	Delegate& operator=(const Delegate& rhs);

	KeyT&	_target;
	fn		_prototype;
};

#endif
