#ifndef UNKNOWN_HPP
# define UNKNOWN_HPP

# include "ASender.hpp"
# include "User.hpp"

# include <string>
# include <ctime>

class Server;

class Unknown : public ASender
{
public:
	Unknown(Server& server, int fd);
	~Unknown(void);

	ssize_t				send(void);
	ssize_t				send(const std::string& data);
	ssize_t				send(const Message& message);

	friend 				User::User(const Unknown& src);

private:
	Unknown&	operator=(const Unknown& rhs);

	std::string			_ident;
	int					_pollIndex;

	void	_updateMask(void);
};

#endif
