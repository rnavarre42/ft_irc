#include "Invite.hpp"
#include <vector>
#include <iostream>

Invite::Invite(void) {}
Invite::~Invite(void) {}

bool	Invite::insert(User *user, Channel *channel)
{
	inviteVector_iterator	it;

	if ((it = this->find(user, channel)) == this->_inviteVector.end())
	{
		this->_inviteVector.push_back(std::make_pair(user, channel));
		return (true);
	}
	return (false);
}

bool	Invite::erase(User *user, Channel *channel)
{
	inviteVector_iterator	it;

	if ((it = this->find(user, channel)) != this->_inviteVector.end())
	{
		this->_inviteVector.erase(it);
		return (true);
	}
	return (false);
}

Invite::inviteVector_iterator	Invite::find(User *user, Channel *channel)
{
	for (inviteVector_iterator it = this->_inviteVector.begin(); it != this->_inviteVector.end(); it++)
		if (it->first == user && it->second == channel)
			return it;
	return this->_inviteVector.end();
}

void	Invite::eraseUser(User *user)
{
	inviteVector_iterator currentIt;

	for (inviteVector_iterator it = this->_inviteVector.begin(); it != this->_inviteVector.end(); )
	{
		currentIt = it;
		if (currentIt->first == user)
			this->_inviteVector.erase(currentIt);
		else
			it++;
	}
}

void	Invite::eraseChannel(Channel *channel)
{
	inviteVector_iterator currentIt;

	for (inviteVector_iterator it = this->_inviteVector.begin(); it != this->_inviteVector.end(); )
	{
		currentIt = it;
		if (currentIt->second == channel)
			this->_inviteVector.erase(currentIt);
		else
			it++;
	}
}
