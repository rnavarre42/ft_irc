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
	bool onChanModeEvent(int order, int sign, Access &access, User &user, Channel &channel, Message &message);
//	void onDisableChanModeEvent(int order, Access &access, User &user, Channel &channel, Message &message);
	void onShowChanModeEvent(void);
};

#endif
