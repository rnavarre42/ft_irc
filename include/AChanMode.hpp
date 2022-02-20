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

	AChanMode(Server &server) : _server(server) {}
	virtual ~AChanMode(void) {}

	virtual void onChanEvent(Access &access, Message &message) = 0;
	virtual bool onChanModeEvent(int pos, int sign, Channel &channel, Message &message) = 0;
//	virtual bool onDisableChanModeEvent(int pos, int sign, Channel &channel, Message &message) = 0;
	virtual void onShowChanModeEvent(void) = 0;

	ChanModeConfig	const &getConfig(void) const
	{
		return _chanModeConfig;
	}

private:
	AChanMode(void);

protected:
	Server			&_server;
	ChanModeConfig	_chanModeConfig;

	bool	isSetMode(Channel &channel, void *value)
	{
		return (channel.mode.isSet(this->_chanModeConfig.mode, value));
	}

	bool	setMode(Channel &channel, void *value)
	{
		return channel.mode.insert(this->_chanModeConfig.mode, value);
	}

	bool	unsetMode(Channel &channel, void *value)
	{
		return channel.mode.erase(this->_chanModeConfig.mode, value);
	}
};

#endif
