#ifndef BANCHANMODE_HPP
# define BANCHANMODE_HPP
 
#include "AChanMode.hpp"
#include "BanChanMode.hpp"

#include <string>

class Message;

class BanChanMode : public AChanMode
{
public:
	BanChanMode(Server &server);
	~BanChanMode(void);

	void 		onChanEvent(Access &access, Message &message);
	bool 		onChanModeEvent(int pos, int sign, Channel &channel, Message &message);
	void 		onShowChanModeEvent(void);
	void 		onDelete(void *pointer);
	std::string getValue(void *pointer);
};

#endif
