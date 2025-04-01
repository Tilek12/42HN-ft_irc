#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>

#include "IChannel.hpp"

class Channel : public IChannel
{
private:
	Channel(const Channel& other) = delete;
	Channel& operator=(const Channel& other) = delete;

	std::string 				_name;
	std::string 				_topic;
	std::vector<std::string>	_users;
	std::vector<std::string>	_operators;
	std::vector<std::string>	_invitedUsers;
	bool						_isInviteOnly;
	bool						_hasPassword;
	std::string					_password;
	size_t						_userLimit;

	bool						_isValidPassword(const std::string password);
public:
	Channel();
	Channel(const std::string& name);
	// Channel(const std::string& name, const std::string& topic, bool isInviteOnly, \
	// 	bool hasPassword, std::string password, size_t userLimit);
	~Channel();

	std::string 				getName() const;
	std::string 				getTopic() const;
	void						setTopic(const std::string& topic);
	std::vector<std::string> 	getUsers() const override;
	void 						addUser(const std::string& user) override;
	void 						removeUser(const std::string& user) override;
	std::vector<std::string> 	getOperators() const override;
	void 						addOperator(const std::string& oper) override;
	void 						removeOperator(const std::string& oper);
	std::vector<std::string> 	getInvitedUsers() const override;
	void						addInvitedUser(const std::string& user);
	void						removeInvitedUser(const std::string& user);
	bool 						getHasPassword() const override;
	void 						setHasPassword(bool hasPassword);
	bool 						getIsInviteOnly() const override;
	void 						setIsInviteOnly(bool isInviteOnly);
	size_t						getUserLimit() const override;
	void						setUserLimit(size_t limit);
	std::string					getPassword() const override;
	void						setPassword(const std::string& password);

	static bool					isValidChannelName(const std::string& name);
	bool 						isUser(const std::string& user) override;
	bool 						isOperator(const std::string& user) override;
	bool 						isInvitedUser(const std::string& user) override;
};

#endif
