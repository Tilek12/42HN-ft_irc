#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>

#include "IServer.hpp"
#include "IClient.hpp"

class Command
{
private:
	Command() = delete;
	Command(Command& other) = delete;
	Command& operator=(Command& other) = delete;
	~Command() = delete;
public:
	static void joinChannelCmd(IClient& client, IServer& server, std::vector<std::string>& joinParams);
};

#endif