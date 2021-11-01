#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <ctime>
# include "User.hpp"
# include "Server.hpp"

class User;

class Channel
{
public:
	Channel(std::string name);
	~Channel(void);

	std::string const					&getName(void) const;
	std::map<std::string, User *> const	&getUserMap(void) const;

	void							setOwner(std::string value);
	std::string const				&getOwner(void) const;
	
	void							setTopic(std::string value);
	std::string	const				&getTopic(void) const;
		
	void							setTopicOwn(std::string value);
	std::string	const				&getTopicOwn(void) const;

	void							setTopicTime(time_t value);
	time_t const					&getTopicTime(void) const;

	void send(std::string msg);
	void join(User user);
	void part(User user);

private:
	std::string						name;
	std::string						topic;
	std::string						owner;
	std::string						topicOwn;
	time_t							topicTime;
	std::map<std::string, User *>	userMap;

	Channel(void);
};

#endif
