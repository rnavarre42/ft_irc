#ifndef READLINE_HPP
# define READLINE_HPP

# include <string>
# include <deque>
# include <termios.h>

class Readline
{
public:
	typedef std::deque<std::string>		stringDeque_type;
	typedef stringDeque_type::iterator	stringDeque_iterator;

	Readline(void);
	~Readline(void);

	std::string &operator()(void);

private:
	Readline(Readline const &src);
	Readline &operator=(Readline const &rhs);

	stringDeque_type		historyDeque;
	std::string				line;
	std::string				copy;
	size_t					index;
	
	struct termios 			old_tio, new_tio;
};

#endif
