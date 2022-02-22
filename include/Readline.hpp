#ifndef READLINE_HPP
# define READLINE_HPP

# include <string>
# include <deque>
# include <vector>
# include <utility>
# include <termios.h>

# define LF_CHR		"\n"
# define DEL_CHR	127


class Readline
{
public:
	typedef std::deque<std::string>		stringDeque_type;
	typedef stringDeque_type::iterator	stringDeque_iterator;

	typedef void(Readline::*f)(std::string const &);

	typedef std::pair<std::string, f>	pair_type;
	typedef std::vector<pair_type>		pairVector_type;

	Readline(void);
	~Readline(void);

	std::string &operator()(void);

private:
	Readline(Readline const &src);
	Readline &operator=(Readline const &rhs);

	void	_newLine(std::string const &data);
	void	_backSpace(std::string const &);

	stringDeque_type		historyDeque;
	std::string				line;
	std::string				copy;
	size_t					index;
	pairVector_type			eventVector;
	
	struct termios 			old_tio, new_tio;
};

#endif
