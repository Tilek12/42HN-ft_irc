#ifndef CHANNELCMDS_HPP
#define CHANNELCMDS_HPP

#include <vector>

#include "IServer.hpp"
#include "../include/IClient.hpp"

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
	static void kickUserCmd(IClient& client, IServer& server, std::vector<std::string>& kickParams);
};

#endif