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
	virtual void onEnableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message) = 0;
	virtual void onDisableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message) = 0;
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

	bool	setMode(Channel &channel, void *value)
	{
		if (channel.mode.findUnique(this->_chanModeConfig.mode, value) == channel.mode.getModeMultimap().end())
		{
			channel.mode.insert(this->_chanModeConfig.mode, value);
			return true;
		}
		return false;
	}
};

#endif
