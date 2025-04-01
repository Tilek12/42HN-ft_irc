#include "../include/Channel.hpp"
#include "../include/IChannel.hpp"
#include "../include/errorReplies.hpp"
#include "../include/channelHelperFcts.hpp"

Channel::Channel()
	:	_name(""), _topic(""), _isInviteOnly(false), \
		_hasPassword(false), _password(""), _userLimit(0)
{
}
Channel::Channel(const std::string& name)
	:	_name(name),  _topic(""), _isInviteOnly(false), \
	_hasPassword(false), _password(""), _userLimit(0)
{

}

// Channel::Channel(const std::string& name, const std::string& topic, \
// 	bool isInviteOnly, bool hasPassword, std::string password, size_t userLimit)
// 	:	_topic(topic), _isInviteOnly(isInviteOnly), \
// 		_hasPassword(hasPassword), _userLimit(userLimit)
// {
// 	if (isValidChannelName(name))
// 		_name = name;
// 	else
// 		std::cerr << "Error code " << ERR_BADCHANMASK << ": bad channel mask. \
// 			Invalid channel name: " << name << std::endl;
// 	if (_hasPassword)
// 	{
// 		if (!_isValidPassword(password))
// 			std::cerr << "Invalid password for channel: " << name << std::endl;
// 	}
// }

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

std::vector<std::string> Channel::getUsers() const
{
	return _users;
}

void Channel::addUser(const std::string& user)
{
	auto it = std::find(_invitedUsers.begin(), _invitedUsers.end(), user);
	if (_isInviteOnly && it == _invitedUsers.end())
    {
        std::cerr << "User " << user << " is not invited to join " << _name << std::endl;
        return;
    }
	if (_userLimit == 0 || _users.size() < _userLimit)
        _users.push_back(user);
    else
        std::cerr << "User limit reached, cannot add " << user << " to channel " << _name << std::endl;
}

void Channel::removeUser(const std::string& user)
{
	removeEntryFromVec(_users, user);
	removeEntryFromVec(_operators, user);
}

std::vector<std::string> Channel::getOperators() const
{
	return _operators;
}

void Channel::addOperator(const std::string& oper)
{
	auto it = std::find(_operators.begin(), _operators.end(), oper);
	if (it == _operators.end())
		_operators.push_back(oper);
}

void Channel::removeOperator(const std::string&oper)
{
	removeEntryFromVec(_operators, oper);
}

std::vector<std::string> Channel::getInvitedUsers() const
{
	return _invitedUsers;
}

void Channel::addInvitedUser(const std::string& user)
{
	_invitedUsers.push_back(user);
}

void Channel::removeInvitedUser(const std::string& user)
{
	removeEntryFromVec(_invitedUsers, user);
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

bool Channel::isValidChannelName(const std::string& name)
{
	if (name.size() > 50 || name.empty())
	{
		return false;
	}
	if (name[0] != '#' && name[0] != '&' && name[0] != '+' && name[0] != '!')
	{
		return false;
	}
	for (size_t i = 0; i < name.size(); i++)
	{
		if (name[i] == ' ' || name[i] == ',' || name[i] == 7)
		{
			return false;
		}
	}
	return true;
}

bool Channel::_isValidPassword(const std::string password)
{
	if (_password == password)
		return true;
	else
		return false;
}

std::string Channel::getPassword() const
{
	return _password;
}

void Channel::setPassword(const std::string& password)
{
	_password = password;
}

bool Channel::isUser(const std::string& user)
{
	return std::find(_users.begin(), _users.end(), user) != _users.end();
}
bool  Channel::isOperator(const std::string& user)
{
	return std::find(_operators.begin(), _operators.end(), user) != _operators.end();
}
bool  Channel::isInvitedUser(const std::string& user)
{
	return std::find(_invitedUsers.begin(), _invitedUsers.end(), user) != _invitedUsers.end();
}
