#include "../include/Channel.hpp"
#include "../include/errorReplies.hpp"

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

Channel::Channel(const std::string& name, const std::string& topic, \
	bool isInviteOnly, bool hasPassword, std::string password, size_t userLimit)
	:	_topic(topic), _isInviteOnly(isInviteOnly), \
		_hasPassword(hasPassword), _userLimit(userLimit)
{
	if (isValidChannelName(name))
		_name = name;
	else
		std::cerr << "Error code " << ERR_BADCHANMASK << ": bad channel mask. \
			Invalid channel name: " << name << std::endl;
	if (_hasPassword)
	{
		if (!_isValidPassword(password))
			std::cerr << "Invalid password for channel: " << name << std::endl;
	}
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
	if (_isInviteOnly && _invitedUsers.find(user) == _invitedUsers.end())
    {
        std::cerr << "User " << user << " is not invited to join " << _name << std::endl;
        return;
    }

	if (_userLimit == 0 || _users.size() < _userLimit)
        _users.insert(user);
    else
        std::cerr << "User limit reached, cannot add " << user << " to channel " << _name << std::endl;
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
	if (_operators.find(oper) == _operators.end())
	{
		_operators.insert(oper);
	}
	
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
	_invitedUsers.insert(user);
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

void	 Channel::setPassword(const std::string& password)
{
	_password = password;
}