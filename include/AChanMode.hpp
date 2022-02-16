#ifndef ICHANMODE_HPP
# define ICHANMODE_HPP
 
#include "ChanModeConfig.hpp"

#include <string>

class Server;
class Message;
class User;
class Channel;

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
	virtual void onEnableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value) = 0;
	virtual void onDisableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value) = 0;
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

};

#endif
