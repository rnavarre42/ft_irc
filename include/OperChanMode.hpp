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
	void onEnableChanModeEvent(Access &access, std::string &value);
	void onDisableChanModeEvent(Access &access, std::string &value);
	void onShowChanModeEvent(void);
};

#endif
