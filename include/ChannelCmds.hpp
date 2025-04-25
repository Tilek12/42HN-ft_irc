#ifndef CHANNELCMDS_HPP
#define CHANNELCMDS_HPP

#include <vector>
#include <algorithm>

#include "IServer.hpp"
#include "../include/IClient.hpp"

class IClient;
class IServer;

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
	static void inviteUserCmd(IClient& client, IServer& server, std::vector<std::string>& inviteParams);
	static void topicUserCmd(IClient& client, IServer& server, std::vector<std::string>& topicParams);
	static void modeChannelCmd(IClient& client, IServer& server, std::vector<std::string>& modeParams);
};

void processJoinRequest(IClient& client, IServer& server, const std::string& channelName, \
    const std::string& password);
bool joinAllowed(IClient& client, IServer& server, IChannel* channel, const std::string& password);
void processPartRequest(IClient& client, IServer& server, \
    const std::string& channelName, const std::string& reason);
void processKickRequest(IClient& client, IServer& server, \
	const std::string& channelName, const std::string& userName, const std::string& reason);
bool isOperatorOnChannel(IClient& client, IServer& server, IChannel* channel);
bool isClientOnChannel(IClient& client, IServer& server, IChannel* channel);
bool isUserOnChannel(IClient& client, IServer& server, IChannel* channel, const std::string& userName);
bool isInvitedUserOnChannel(IClient& client, IServer& server, IChannel* channel, const std::string& userName);
void processInviteRequest(IClient& client, IServer& server, IChannel* channel, \
    const std::string& userName);
bool canOnlyOperatorChangeTopic(IClient& client, IServer& server, IChannel* channel);
void processGetTopicRequest(IClient& client, IServer& server, IChannel* channel);
void processSetTopicRequest(IClient& client, IServer& server, IChannel* channel, std::string newTopic);
void processModeTwoArgsRequest(IClient& client, IServer& server, IChannel* channel, std::string mode);
void processModeThreeArgsRequest(IClient& client, IServer& server, IChannel* channel, std::string mode, \
	std::string modeParamIdx2);
void channelCmdHandler(IClient& client, IServer& server, std::vector<std::string>& args);
void handleNewChannel(IServer& server, IChannel*& channel,
	const std::string& channelName,
	const std::string& password, IClient& client);
void sendNameReplies(Client* client, IChannel* channel, IServer& server);
void sendModeTwoArgsMsg(IClient& client, IServer& server, \
    IChannel* channel, std::string mode);
void sendModeThreeArgsMsg(IClient& client, IServer& server, \
    IChannel* channel, std::string mode, std::string modeParamIdx2);
void sendErrorMode(IClient& client, IServer& server, std::string mode);

#endif
