#ifndef INVITE_HPP
# define INVITE_HPP

# include <vector>
# include <cstddef>

class ASender;
class Channel;

class Invite
{
public:
	typedef std::pair<ASender*, Channel*>		senderChannelPair_type;
	typedef std::vector<senderChannelPair_type>	inviteVector_type;
	typedef inviteVector_type::iterator			inviteVector_iterator;

	Invite(void);
	~Invite(void);

	bool					insert(ASender& sender, Channel& channel);
	inviteVector_iterator	find(ASender& sender, Channel& channel);
	bool					erase(ASender& sender, Channel& channel);
	void					erase(Channel& channel);
	void					erase(ASender& sender);

	inviteVector_iterator	begin(void)
	{
		return this->_inviteVector.begin();
	}

	inviteVector_iterator	end(void)
	{
		return this->_inviteVector.end();
	}

private:
	inviteVector_type					_inviteVector;
};

#endif
