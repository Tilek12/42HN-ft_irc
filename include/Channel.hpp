#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <set>

class Channel
{
private:
	std::string 			_name;
	std::string 			_topic;
	std::set<std::string>	_users;
	std::set<std::string>	_operators;
	std::set<std::string>	_invitedUsers;
	bool					_isInviteOnly;
	bool					_hasPassword;
	size_t					_userLimit;
public:
	Channel();
	Channel(const std::string name);
	Channel(const Channel& other);
	Channel& operator=(const Channel& other);
	~Channel();

	std::string 			getName();
	std::string 			getTopic();
	void					setTopic(std::string topic);
	std::set<std::string> 	getUsers();
	void 					addUser(std::string user);
	void 					removeUser(std::string user);
	std::set<std::string> 	getOperators();
	void 					addOperator(std::string operator);
	void 					removeOperator(std::string operator);
	std::set<std::string> 	getInvitedUsers();
	void					addInvitedUsers();
	void					removeInvitedUsers();
	std::set<std::string> 	getInvitedUsers();
	bool 					hasPassword();
	bool 					setHasPassword();
	bool 					getIsInviteOnly();
	void 					setIsInviteOnly();
	size_t					getUserLimit();
	void					setUserLimit();
};

#endif