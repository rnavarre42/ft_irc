#ifndef BANCHANMODE_HPP
# define BANCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class BanChanMode : public AChanMode
{
public:
	BanChanMode(Server &server);
	~BanChanMode(void);

	void onChanEvent(Access &access, Message &message);
	void onEnableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value);
	void onDisableChanModeEvent(Access &access, User &user, Channel &channel, std::string &value);
	void onShowChanModeEvent(void);
};

#endif
