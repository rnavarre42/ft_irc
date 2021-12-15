#ifndef ICHANMODE_HPP
# define ICHANMODE_HPP
 
#include "ChanModeConfig.hpp"

#include <string>

class Server;
class Message;

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

	virtual void onChanEvent(Access &modeAccess, Message &message) = 0;
	virtual void onEnableChanModeEvent(Access &modeAccess, std::string &value) = 0;
	virtual void onDisableChanModeEvent(Access &modeAccess, std::string &value) = 0;
	virtual void onShowChanModeEvent(void) = 0;

	ChanModeConfig	const &getConfig(void) const
	{
		return _chanModeConfig;
	}

protected:
	Server			&_server;
	ChanModeConfig	_chanModeConfig;

};

#endif
