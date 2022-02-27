#ifndef SECRETCHANMODE_HPP
# define SECRETCHANMODE_HPP
 
# include "AChanMode.hpp"

# include <string>

class Message;

class SecretChanMode : public AChanMode
{
public:
	SecretChanMode(Server& server);
	~SecretChanMode(void);

	void	onChanEvent(Access& access, int event, Message& message, int& numeric);
	bool	onChanModeEvent(int pos, int sign, Channel& channel, Message& message);
	void	onShowChanModeEvent(void);

	void		onDelete(void* pinter);
	std::string	toString(void* pointer);
};

#endif
