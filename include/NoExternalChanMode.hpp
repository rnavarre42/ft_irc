#ifndef NOEXTERNALCHANMODE_HPP
# define NOEXTERNALCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class NoExternalChanMode : public AChanMode
{
public:
	NoExternalChanMode(Server &server);
	~NoExternalChanMode(void);

	void onChanEvent(Access &access, Message &message);
	void onEnableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value);
	void onDisableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value);
	void onShowChanModeEvent(void);
};

#endif