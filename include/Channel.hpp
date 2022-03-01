#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"
# include "User.hpp"

# include <sstream>
# include <string>
# include <map>
# include <ctime>

# define CHANNEL_CREATE		0x0001
# define CHANNEL_JOIN		0x0002
# define CHANNEL_BADPREFIX	0x0004
# define CHANNEL_ISALREADY	0x0008
# define CHANNEL_BADNAME	0x0010
# define CHANNEL_PART		0x0020
# define CHANNEL_DESTROY	0x0040
# define CHANNEL_NOTPRESENT	0x0080
# define CHANNEL_CANTJOIN	0x0100
# define CHANNEL_CANTPART	0x0200
# define CHANNEL_ISFULL		0x0400
# define CHANNEL_PRIVATE	0x0800
# define CHANNEL_PASSWORD	0x1000
# define CHANNEL_BANNED		0x2000

class Channel
{
public:
	typedef std::map<std::string, Channel*>		channelMap_type;
	typedef channelMap_type::iterator			channelMap_iterator;

	Channel(const std::string& name, const User& user, Server& server);
	~Channel(void);

	struct TopicInfo
	{
		TopicInfo(void)
		{
			this->setTopic();
		}

		TopicInfo(const std::string& own, const std::string& topic)
		{
			this->setTopic(own, topic);
		}

		~TopicInfo()
		{}

		void setTopic(const std::string& own = "", const std::string& topic = "")
		{
			std::ostringstream	oss;

			oss << std::time(NULL);
			this->own = own;
			this->topic = topic;
			this->time = oss.str();
		}

		std::string own;
		std::string	topic;
		std::string	time;

	private:
		TopicInfo(const TopicInfo& src);
		TopicInfo &operator=(const TopicInfo& rhs);
	};

	class Mode
	{
	public:
		typedef std::multimap<char, void*>						multimap_type;
		typedef multimap_type::iterator							multimap_iterator;
		typedef multimap_type::const_iterator					multimap_const_iterator;
		typedef std::pair<multimap_iterator, multimap_iterator>	rangePairMultimap_type;

		Mode(void)
		{}

		~Mode(void)
		{}

		void*&				operator[](char chr);
		multimap_iterator	find(char chr);

		multimap_type const&	getModeMultimap(void)
		{
			return this->_modeMultimap;
		}

		multimap_iterator		begin(void);
		multimap_iterator		end(void);
		multimap_const_iterator	begin(void) const;
		multimap_const_iterator	end(void) const;

		bool					isSet(const char modeName);
		bool					isSet(const char modeName, const void* value) ;
		rangePairMultimap_type	getList(const char modeName);
		bool					insert(char modeName, void* value);
		bool					erase(const char modeName, const void* value);
		bool					erase(const char modeName);
		void					erase(multimap_iterator pos);
		multimap_iterator		findUnique(const char modeName, const void* value);

	private:
		Mode(Mode const &src);
		Mode&	operator=(Mode const &rhs);

		multimap_type				_modeMultimap;

	}								mode;
	const std::string&				getName(void) const;
	bool							empty(void);

	bool							isOper(const ASender* sender);
	bool							isVoice(const ASender* sender);

	void							setTopicInfo(const std::string& own, const std::string& topic);
	const TopicInfo&				getTopicInfo(void) const;

	void							setOwner(const std::string& value);
	const std::string&				getOwner(void) const;

	Server::userMap_insert			insert(User* user);
	Server::userMap_insert			insert(const std::string& userName, User* user);
	void							erase(const User* user);
	void							erase(const std::string& userName);
	
	Server::userMap_iterator	begin(void)
	{
		return this->_userMap.begin();
	}

	Server::userMap_iterator	end(void)
	{
		return this->_userMap.end();
	}
	
	Server::userMap_type::size_type	size(void)
	{
		return this->_userMap.size();
	}

	Server::userMap_iterator	find(const std::string& userName)
	{
		return this->_userMap.find(strToUpper(userName));
	}

	User*&	operator[](const std::string& userName)
	{
		return this->_userMap.at(strToUpper(userName));
	}
	User*	at(const std::string& userName)
	{
		Server::userMap_iterator	it;

		if ((it = this->find(userName)) == this->_userMap.end())
			return NULL;
		return it->second;
	}

	void send(std::string msg);
	void join(User user);
	void part(User user);

private:
	Channel(void);
	Channel(const Channel& src);
	Channel&	operator=(const Channel& rhs);

	std::string						_name;
	TopicInfo						_topicInfo;	
	std::string						_owner;
	std::string						_topicOwn;
	time_t							_topicTime;
	Server::userMap_type			_userMap;
	Server&							_server;
};

#endif
