#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <Server.hpp>

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

class User;

class Channel
{
public:
	typedef std::map<std::string, Channel *>	channelMap_type;
	typedef channelMap_type::iterator			channelMap_iterator;

	Channel(std::string name, User &user);
	~Channel(void);
	
	class Mode
	{
	public:
		typedef std::multimap<char, void *>						multimap_type;
		typedef multimap_type::iterator							multimap_iterator;
		typedef std::pair<multimap_iterator, multimap_iterator>	rangePairMultimap_type;

		Mode(void) {}
		~Mode(void) {}

		void *operator[](char chr);

		multimap_type const		&getModeMultimap(void)
		{
			return this->_modeMultimap;
		}

		bool					isSet(char modeName);
		bool					isSet(char modeName, void *value);
		rangePairMultimap_type	getList(char modeName);
		bool					insert(char modeName, void *value);
		bool					erase(char modeName, void *value);
		bool					erase(char modeName);
		void					erase(multimap_iterator pos);
		multimap_iterator		findUnique(char modeName, void *value);
		

	private:
		multimap_type						_modeMultimap;

	}								mode;
	std::string const				&getName(void) const;
	Server::userMap_type			&getUserMap(void);
	void							insertUser(User *user);
	void							eraseUser(std::string value);
	bool							empty(void);

	bool							isOper(User *user);
	bool							isVoice(User *user);

	void							setOwner(std::string value);
	std::string const				&getOwner(void) const;
	
	void							setTopic(std::string value);
	std::string	const				&getTopic(void) const;
		
	void							setTopicOwn(std::string value);
	std::string	const				&getTopicOwn(void) const;

	void							setTopicTime(time_t value);
	time_t const					&getTopicTime(void) const;

	inline Server::userMap_iterator	userFind(std::string &userName)
	{
		return this->_userMap.find(strToUpper(userName));
	}

	void send(std::string msg);
	void join(User user);
	void part(User user);

private:
	std::string						_name;
	std::string						_topic;
	std::string						_owner;
	std::string						_topicOwn;
	time_t							_topicTime;
	Server::userMap_type			_userMap;

	Channel(void);
};

#endif
