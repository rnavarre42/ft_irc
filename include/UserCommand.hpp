#include "CommandBase.hpp"
#include <string>

class UserCommand : CommandBase
{
	public:
		UserCommand(Server &server, char level);

		virtual void	exec(void);
};
