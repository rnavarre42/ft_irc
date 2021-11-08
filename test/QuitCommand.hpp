#ifndef QUITCOMMAND_HPP
# define QUITCOMMAND_HPP

# include "ACommand.hpp"

class	QuitCommand : public ACommand
{
public:
	friend class Server;

	void	exec(Server &server);

private:
	void	_load(Server &server);
};

#endif
