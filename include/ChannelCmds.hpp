#ifndef CHANNELCMDS_HPP
#define CHANNELCMDS_HPP

#include <vector>

#include "IServer.hpp"
#include "../src/client/IClient.hpp"

class ChannelCmds
{
private:
	ChannelCmds() = delete;
	ChannelCmds(ChannelCmds& other) = delete;
	ChannelCmds& operator=(ChannelCmds& other) = delete;
	~ChannelCmds() = delete;
public:
	static void joinChannelCmd(IClient& client, IServer& server, std::vector<std::string>& joinParams);
	static void partChannelCmd(IClient& client, IServer& server, std::vector<std::string>& partParams);
};

#endif