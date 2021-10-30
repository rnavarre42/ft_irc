#include "ACommand.hpp"
#include <string>

class PrivmsgCommand : public ACommand
{
	public:
		PrivmsgCommand(Server &server, int accessLevel, int paramCount);

	private:
		virtual void	_exec(Message &message);
};
