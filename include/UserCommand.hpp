#include "ACommand.hpp"
#include <string>

class UserCommand : public ACommand
{
	public:
		UserCommand(Server &server, char level);

		virtual void	exec(Message &message);
};
