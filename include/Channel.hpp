#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <set>

class Channel
{
private:
	Channel(const Channel& other) = delete;
	Channel& operator=(const Channel& other) = delete;

	std::string 			_name;
	std::string 			_topic;
	std::set<std::string>	_users;
	std::set<std::string>	_operators;
	std::set<std::string>	_invitedUsers;
	bool					_isInviteOnly;
	bool					_hasPassword;
	std::string				_password;
	size_t					_userLimit;
	
	bool					_isValidPassword(const std::string password);
public:
	Channel();
	Channel(const std::string& name, const std::string& topic, bool isInviteOnly, \
		bool hasPassword, std::string password, size_t userLimit);
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
	std::string				getPassword() const;
	void					setPassword(const std::string& password);
	
	static bool				isValidChannelName(const std::string& name);
};

#endif