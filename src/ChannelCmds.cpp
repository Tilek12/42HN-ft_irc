#include <vector>
#include <iostream>
#include <string>

#include "../include/IRCerror.hpp"
#include "../include/Channel.hpp"
#include "../include/ChannelCmds.hpp"
#include "../include/channelHelperFcts.hpp"
#include "../include/Client.hpp"
#include "../include/client_c_h.hpp"

#include "../include/IServer.hpp"
#include "../include/IChannel.hpp"
#include "../include/IClient.hpp"

void ChannelCmds::joinChannelCmd(IClient& client, IServer& server, std::vector<std::string>& joinParams)
{
    std::vector<std::string> channelNames;
    std::vector<std::string> passwords;
    std::string password;

    if (joinParams.empty())
    {
        std::cerr << "Error code " << IRCerror::ERR_NEEDMOREPARAMS << " JOIN: not enough joinParams" << std::endl;
        return;
    }
    channelNames = parseCommaString(joinParams[0]);
    if (joinParams.size() > 1)
        passwords = parseCommaString(joinParams[1]);
    for (size_t i = 0; i < channelNames.size(); ++i)
    {
        if (i < passwords.size())
            password = passwords[i];
        else
            password = "";
            processJoinRequest(client, server, channelNames[i], password);
    }
}

void ChannelCmds::partChannelCmd(IClient& client, IServer& server, std::vector<std::string>& partParams)
{
    std::vector<std::string> channelNames;
    std::string reason = "";

    if (partParams.empty())
    {
        std::cerr << "Error code " << IRCerror::ERR_NEEDMOREPARAMS << " PART: not enough partParams" << std::endl;
        return;
    }
    channelNames = parseCommaString(partParams[0]);
    if (partParams.size() == 2)
        reason = partParams[1];
    for (size_t i = 0; i < channelNames.size(); ++i)
    {
       processPartRequest(client, server, channelNames[i], reason);
    }
}

void ChannelCmds::kickUserCmd(IClient& client, IServer& server, std::vector<std::string>& kickParams)
{
    std::vector<std::string> channelNames;
    std::vector<std::string> userNames;
    std::string userName;
    std::string reason = "";

    if (kickParams.size() < 2)
    {
        std::cerr << "Error code " << IRCerror::ERR_NEEDMOREPARAMS << " KICK: not enough kickParams" << std::endl;
        return;
    }
    channelNames = parseCommaString(kickParams[0]);
    userNames = parseCommaString(kickParams[1]);
    if (kickParams.size() == 3)
        reason = kickParams[2];
    for (size_t i = 0; i < channelNames.size(); i++)
    {
        if (i < userNames.size())
            userName = userNames[i];
        else
            userName = "";
        processKickRequest(client, server, channelNames[i], userName, reason);
    }
}

void ChannelCmds::inviteUserCmd(IClient& client, IServer& server, std::vector<std::string>& inviteParams)
{
    if (inviteParams.size() < 2)
    {
        std::cerr << "Error code " << IRCerror::ERR_NEEDMOREPARAMS << \
            " INVITE: not enough inviteParams" << std::endl;
        return;
    }
    std::string user = inviteParams[0];
    std::string channelName = inviteParams[1];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        CommandHandler::SendMessage(&client, "Error code " + \
            std::string(IRCerror::ERR_NOSUCHCHANNEL) + " : non existing channel " + channelName);
        return;
    }
    if (!(channel->getIsInviteOnly()))
    {
        CommandHandler::SendMessage(&client, "Error: no isInvitedOnly channel " \
            + channelName);
        return;
    }
    if (!isOperatorOnChannel(client, channel))
		return;
    if (isUserOnChannel(client, channel, user))
        return;
    if (isInvitedUserOnChannel(client, channel, user))
        return;
    processInviteRequest(client, channel, user);
}

void ChannelCmds::topicUserCmd(IClient& client, IServer& server, std::vector<std::string>& topicParams)
{
    if (topicParams.empty())
    {
        std::cerr << "Error code " << IRCerror::ERR_NEEDMOREPARAMS << " TOPIC: not enough topicParams" << std::endl;
        return;
    }
    std::string channelName = topicParams[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        CommandHandler::SendMessage(&client, "Error code " + \
            std::string(IRCerror::ERR_NOSUCHCHANNEL) + " : non existing channel " + channelName);
        return;
    }
    if (canOnlyOperatorChangeTopic(client, channel))
       return;
    if (topicParams.size() == 1)
        processGetTopicRequest(client, channel);
    std::string newTopic = topicParams[1];
    if (topicParams.size() == 2 && newTopic.front() == ':')
        processSetTopicRequest(client, channel, newTopic);
}

void ChannelCmds::modeChannelCmd(IClient& client, IServer& server, std::vector<std::string>& modeParams)
{
    if (modeParams.size() < 2)
    {
        std::cerr << "Error code " << IRCerror::ERR_NEEDMOREPARAMS << " MODE: not enough modeParams" << std::endl;
        return;
    }
    std::string channelName = modeParams[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        CommandHandler::SendMessage(&client, "Error code " + \
            std::string(IRCerror::ERR_NOCHANMODES) + " : non existing channel " + channelName);
        return;
    }
    if (!isOperatorOnChannel(client, channel))
        return;
    std::string mode = modeParams[1];
    if (modeParams.size() == 2)
        processModeTwoArgsRequest(client, channel, mode);
    else if (modeParams.size() == 3)
        processModeThreeArgsRequest(client, channel, mode, modeParams[2]);
}
