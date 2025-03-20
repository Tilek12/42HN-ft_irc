#include "../include/Channel.hpp"

Channel::Channel()
	:	_name(""), _topic(""), _isInviteOnly(false), \
		_hasPassword(false), _userLimit(100)
{
}

Channel::Channel(const std::string name)
	:	_name(name)
{
}

Channel::Channel(const Channel& other)
	:	_name(other._name)
{
}

Channel& Channel::operator=(const Channel& other)
{
	if (this != &other)
	{
		_name = other._name;
	}
	return *this;
}

Channel::~Channel()
{
}