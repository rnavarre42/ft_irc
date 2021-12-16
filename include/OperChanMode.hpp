#ifndef OPERCHANMODE_HPP
# define OPERCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class OperChanMode : public AChanMode
{
public:
	OperChanMode(Server &server);
	~OperChanMode(void);

	void onChanEvent(Access &access, Message &message);
	void onEnableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value);
	void onDisableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value);
	void onShowChanModeEvent(void);
};

#endif
