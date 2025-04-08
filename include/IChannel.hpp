#ifndef ICHANNEL_HPP
#define ICHANNEL_HPP

#include <iostream>
#include <vector>

class IChannel
{
public:
	virtual ~IChannel() {}

	virtual void addOperator(const std::string& oper) = 0;
	virtual bool getHasPassword() const = 0;
	virtual std::string	getPassword() const = 0;
	virtual size_t getUserLimit() const = 0;
	virtual void addUser(const std::string& user) = 0;
	virtual std::vector<std::string> getInvitedUsers() const = 0;
	virtual bool getIsInviteOnly() const = 0;
	virtual std::vector<std::string> getUsers() const = 0;
	virtual void removeUser(const std::string& user) = 0;
	virtual std::vector<std::string> getOperators() const = 0;
	virtual	bool isUser(const std::string& user) = 0;
	virtual	bool isOperator(const std::string& user) = 0;
	virtual	bool isInvitedUser(const std::string& user) = 0;
	virtual bool isValidPassword(const std::string password) = 0;
	virtual std::string getName() const = 0;
	virtual void addInvitedUser(const std::string& user) = 0;
	virtual bool getOnlyOperatorCanChangeTopic() = 0;
	virtual std::string getTopic() const = 0;
	virtual void setTopic(const std::string& topic) = 0;
	virtual void setIsInviteOnly(bool isInviteOnly) = 0;
	virtual void setOnlyOperatorCanChangeTopic(bool OnlyOperatorCanChangeTopic) = 0;
	virtual void setUserLimit(size_t limit) = 0;
	virtual void removeOperator(const std::string& oper) = 0;
	virtual void setHasPassword(bool hasPassword) = 0;
	virtual void setPassword(const std::string& password) = 0;
	  
	
};

#endif
