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
	ssize_t				send(const Message& message);
	ssize_t				send(const std::string& data);

	friend 				User::User(const Unknown& src);

private:
	Unknown&	operator=(const Unknown& rhs);

	void	_updateMask(void);
};

#endif
