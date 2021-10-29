#include "ACommand.hpp"
#include <string>

class UserCommand : public ACommand
{
	public:
		UserCommand(Server &server, char level);

	private:
		virtual void	_exec(Message &message);
};
