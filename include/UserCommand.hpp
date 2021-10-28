#include "ACommand.hpp"
#include <string>

class UserCommand : ACommand
{
	public:
		UserCommand(Server &server, char level);

		virtual void	exec(void);
};
