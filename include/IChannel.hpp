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
	virtual	bool isUser(const std::string& user);
	virtual	bool isOperator(const std::string& user);
	virtual	bool isInvitedUser(const std::string& user);
};

#endif
