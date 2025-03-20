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
	Channel(const std::string& name, const std::string& topic, bool isInviteOnly, \
		bool hasPassword, size_t userLimit);
	Channel(const Channel& other);
	Channel& operator=(const Channel& other);
	~Channel();

	std::string 			getName() const;
	std::string 			getTopic() const;
	void					setTopic(const std::string& topic);
	std::set<std::string> 	getUsers() const;
	void 					addUser(const std::string& user);
	void 					removeUser(const std::string& user);
	std::set<std::string> 	getOperators() const;
	void 					addOperator(const std::string& oper);
	void 					removeOperator(const std::string& oper);
	std::set<std::string> 	getInvitedUsers() const;
	void					addInvitedUser(const std::string& user);
	void					removeInvitedUser(const std::string& user);
	bool 					getHasPassword() const;
	void 					setHasPassword(bool hasPassword);
	bool 					getIsInviteOnly() const;
	void 					setIsInviteOnly(bool isInviteOnly);
	size_t					getUserLimit() const;
	void					setUserLimit(size_t limit);
};

#endif