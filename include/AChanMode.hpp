#ifndef ICHANMODE_HPP
# define ICHANMODE_HPP
 
#include "ChanModeConfig.hpp"
#include "Channel.hpp"

#include <string>

class Server;
class Message;
class User;

class AChanMode
{
public:
	enum Access
	{
		def,
		deny,
		allow
	};

	AChanMode(Server& server) : _server(server) {}
	virtual ~AChanMode(void) {}

	virtual void		onChanEvent(Access& access, int event, Message& message, int& numeric) = 0;
	virtual bool		onChanModeEvent(int pos, int sign, Channel& channel, Message& message) = 0;
	virtual void 		onShowChanModeEvent(void) = 0;
	virtual void 		onDelete(void* pointer) = 0;
	virtual std::string	toString(void* pointer) = 0;

	const ChanModeConfig&	getConfig(void) const
	{
		return _chanModeConfig;
	}

private:
	AChanMode(void);
	AChanMode(const AChanMode& src);
	AChanMode &operator=(const AChanMode& rhs);

protected:
	Server&			_server;
	ChanModeConfig	_chanModeConfig;

	bool	isSetMode(Channel& channel)
	{
		return (channel.mode.isSet(this->_chanModeConfig.mode));
	}

	bool	isSetMode(Channel& channel, void* value)
	{
		return (channel.mode.isSet(this->_chanModeConfig.mode, value));
	}

	bool	setMode(Channel& channel, void* value)
	{
		if (this->_chanModeConfig.unique)
		{
			channel.mode[this->_chanModeConfig.mode] = value;
			return true;
		}
		return channel.mode.insert(this->_chanModeConfig.mode, value);
	}

	bool	unsetMode(Channel& channel, void* value)
	{
		this->onDelete(value);
		return channel.mode.erase(this->_chanModeConfig.mode, value);
	}

	bool	unsetMode(Channel& channel)
	{
		return channel.mode.erase(this->_chanModeConfig.mode);
	}

	void*	getMode(Channel& channel)
	{
		return channel.mode.find(this->_chanModeConfig.mode)->second;
	}
};

#endif
