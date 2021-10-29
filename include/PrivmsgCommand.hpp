#include "ACommand.hpp"
#include <string>

class PrivmsgCommand : public ACommand
{
	public:
		PrivmsgCommand(Server &server, char level);

	private:
		virtual void	_exec(Message &message);
};
