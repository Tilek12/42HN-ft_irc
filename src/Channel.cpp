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

std::string Channel::getName() const
{
	return _name;
}

std::string Channel::getTopic() const
{
	return _topic;
}

void Channel::setTopic(const std::string& topic)
{
	_topic = topic;
}

std::set<std::string> Channel::getUsers() const
{
	return _users;
}

void Channel::addUser(const std::string& user)
{
	_users.insert(user);
}

void Channel::removeUser(const std::string& user)
{
	_users.erase(user);
}

std::set<std::string> Channel::getOperators() const
{
	return _operators;
}

void Channel::addOperator(const std::string& oper)
{
	_operators.insert(oper);
}

void Channel::removeOperator(const std::string&oper)
{
	_operators.erase(oper);
}

std::set<std::string> Channel::getInvitedUsers() const
{
	return _invitedUsers;
}

void Channel::addInvitedUser(const std::string& user)
{
	_users.insert(user);
}

void Channel::removeInvitedUser(const std::string& user)
{
	_invitedUsers.erase(user);
}

bool Channel::getHasPassword() const
{
	return _hasPassword;
}

void Channel::setHasPassword(bool hasPassword)
{
	_hasPassword = hasPassword;
}

bool Channel::getIsInviteOnly() const
{
	return _isInviteOnly;
}

void Channel::setIsInviteOnly(bool isInviteOnly)
{
	_isInviteOnly = isInviteOnly;
}

size_t Channel::getUserLimit() const
{
	return _userLimit;
}

void Channel::setUserLimit(size_t userLimit)
{
	_userLimit = userLimit;
}