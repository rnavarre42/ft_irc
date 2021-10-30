#include "ACommand.hpp"
#include <string>

class UserCommand : public ACommand
{
	public:
		UserCommand(Server &server, int accessLevel, int paramCount);

	private:
		virtual void	_exec(Message &message);
};
