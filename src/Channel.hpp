#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <ctime>
# include "User.hpp"
# include "Server.hpp"

class Channel
{
	public:
		Channel(std::string name);
		~Channel(void);

		std::string						&getName(void) const;
		std::map<std::string, User *>	&getUsers(void) const;

		void							setCreator(std::string value);
		std::string						&getCreator(void) const;
	
		void							setTopic(std::string value);
		std::string						&getTopic(void) const;
		
		void							setTopicOwn(std::string value);
		std::string						&getTopicOwn(void) const;

		void							setTopicTime(time_t value);
		time_t							&getTopicTime(void) const;

		void send(std::string msg);
		void join(User user);
		void part(User user);
	private:
		std::string						name;
		std::string						topic;
		std::string						creator;
		std::string						topicOwn;
		time_t							topicTime;
		std::map<std::string, User *>	users;

		Channel(void);
}

#endif
