#ifndef TOPICCHANMODE_HPP
# define TOPICCHANMODE_HPP
 
#include "AChanMode.hpp"

#include <string>

class Message;

class TopicChanMode : public AChanMode
{
public:
	TopicChanMode(Server &server);
	~TopicChanMode(void);

	void onChanEvent(Access &access, Message &message);
	void onEnableChanModeEvent(Access &access, std::string &value);
	void onDisableChanModeEvent(Access &access, std::string &value);
	void onShowChanModeEvent(void);
};

#endif
