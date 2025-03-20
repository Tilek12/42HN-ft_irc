#include "../include/Channel.hpp"

Channel::Channel()
	:	_name(""), _topic(""), _isInviteOnly(false), \
		_hasPassword(false), _userLimit(0)
{
}

Channel::Channel(const std::string& name, const std::string& topic, \
	bool isInviteOnly, bool hasPassword, size_t userLimit)
	:	_name(name), _topic(topic), _isInviteOnly(isInviteOnly), \
		_hasPassword(hasPassword), _userLimit(userLimit)
{
}

Channel::Channel(const Channel& other)
{
	*this = other;
}

Channel& Channel::operator=(const Channel& other)
{
	if (this != &other)
	{
		_name = other._name;
		_topic = other._topic;
		_isInviteOnly = other._isInviteOnly;
		_hasPassword = other._hasPassword;
		_userLimit = other._userLimit;
		_users = other._users;
		_operators = other._operators;
		_invitedUsers = other._invitedUsers;
	}
	return *this;
}

Channel::~Channel()
{
}