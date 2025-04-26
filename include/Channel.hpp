#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>
#include <algorithm>

#include "IChannel.hpp"

class IServer;

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
	bool						_onlyOperatorCanChangeTopic;

public:
	Channel();
	Channel(const std::string& name);
	~Channel();

	std::string 				getName() const override;
	std::string 				getTopic() const override;
	void						setTopic(const std::string& topic) override;
	std::vector<std::string> 	getUsers() const override;
	std::vector<std::string> 	&getUsers();
	void 						addUser(const std::string& user, IServer& server) override;
	void 						removeUser(const std::string& user) override;
	std::vector<std::string> 	getOperators() const override;
	void 						addOperator(const std::string& oper) override;
	void 						removeOperator(const std::string& oper) override;
	std::vector<std::string> 	getInvitedUsers() const override;
	void						addInvitedUser(const std::string& user) override;
	void						removeInvitedUser(const std::string& user);
	bool 						getHasPassword() const override;
	void 						setHasPassword(bool hasPassword) override;
	bool 						getIsInviteOnly() const override;
	void 						setIsInviteOnly(bool isInviteOnly) override;
	size_t						getUserLimit() const override;
	void						setUserLimit(size_t limit) override;
	std::string					getPassword() const override;
	void						setPassword(const std::string& password) override;
	bool						getOnlyOperatorCanChangeTopic() override;
	void						setOnlyOperatorCanChangeTopic(bool OnlyOperatorCanChangeTopic) override;

	bool						isValidPassword(const std::string password) override;
	static bool					isValidChannelName(const std::string& name);
	bool 						isUser(const std::string& user) override;
	bool 						isOperator(const std::string& user) override;
	bool 						isInvitedUser(const std::string& user) override;
};

#endif
