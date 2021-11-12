#include <map>
#include <iostream>
#include <string>

class DelegateBase
{
public:
	virtual ~DelegateBase(void) {}
	virtual void invoke(void *data) = 0;
};

template <class T, class DataT>
class Delegate : public DelegateBase
{
public:
	typedef void (T::*fn)(DataT *);
	 Delegate(T& target, fn member) : _target(target), _member(member)
	{}
	~Delegate(void){}

	void invoke(void *data)
	{
		(_target.*_member)(static_cast<DataT *>(data));
	}

private:

	T&	_target;
	fn	_member;
};

class CommandBase
{
public:
	virtual ~CommandBase(void) {}
	virtual void _sendClient(void *data) = 0;
};

class JoinCommand : public CommandBase
{
public:
	~JoinCommand(void){}

	void joinEvent(std::string *data)
	{
		std::cout << "JoinCommand.quitEvent << " << *data << std::endl;
	}

	void _sendClient(void *data)
	{
		(void)data;
	}
};

class PartCommand : public CommandBase
{
public:
	~PartCommand(void){}

	void partEvent(std::string *data)
	{
		std::cout << "Que te pires!! << " << *data << std::endl;
	}

	void _sendClient(void *data)
	{
		(void)data;
	}
};

template <class KeyT, class DataT>
class EventHandler
{
public:
	typedef typename std::multimap<KeyT, DelegateBase &>	multimap_type;
	typedef typename multimap_type::iterator				iterator_type;
	void	add(KeyT key, DelegateBase &delegate)
	{
		this->_delegateMap.insert(typename std::pair<KeyT, DelegateBase &>(key, delegate));
	}

	void	raise(KeyT key, DataT &data)
	{
		std::pair<iterator_type, iterator_type> ret;

		ret = this->_delegateMap.equal_range(key);
		for (iterator_type it = ret.first; it != ret.second; ++it)
		{
			std::cout << "delegate.invoke " << key << " ";
			it->second.invoke(&data);
		}
	}

private:
	multimap_type	_delegateMap;
};

int main(void)
{
	EventHandler<int, std::string>	eventHandler;
	std::string	data("mensaje de prueba");
	JoinCommand	join;
	PartCommand	part;

	Delegate<JoinCommand, std::string>	JoinDelegate(join, &JoinCommand::joinEvent);
	Delegate<PartCommand, std::string>	PartDelegate(part, &PartCommand::partEvent);
	
	eventHandler.add(10, JoinDelegate);
	eventHandler.add(11, JoinDelegate);
	eventHandler.add(10, PartDelegate);
	eventHandler.raise(10, data);
	return 0;
}
