/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCmds.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llacsivy <llacsivy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:45:55 by llacsivy          #+#    #+#             */
/*   Updated: 2025/04/26 17:45:58 by llacsivy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void ChannelCmds::joinChannelCmd(IClient& client, IServer& server, \
    std::vector<std::string>& joinParams)
{
    std::vector<std::string> channelNames;
    std::vector<std::string> passwords;
    std::string password;

    if (joinParams.empty())
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS + \
            " " + client.getNickname() + " JOIN :Not enough parameters\r\n";
        server.sendToClient(client.getNickname(), errorMsg);
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

void ChannelCmds::partChannelCmd(IClient& client, IServer& server, \
    std::vector<std::string>& partParams)
{
    std::vector<std::string> channelNames;
    std::string reason = "";

    if (partParams.empty())
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS + \
            " " + client.getNickname() + " PART :Not enough parameters";
        server.sendToClient(client.getNickname(), errorMsg);
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

void ChannelCmds::kickUserCmd(IClient& client, IServer& server, \
    std::vector<std::string>& kickParams)
{
    std::vector<std::string> channelNames;
    std::vector<std::string> userNames;
    std::string userName;
    std::string reason = "";

    if (kickParams.size() < 2)
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS \
            + " " + client.getNickname() + " KICK :Not enough parameters";
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

void ChannelCmds::inviteUserCmd(IClient& client, IServer& server, \
    std::vector<std::string>& inviteParams)
{
    if (inviteParams.size() < 2)
    {
        std::string msg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS + \
            " " + client.getNickname() + " INVITE :Not enough parameters";
        server.sendToClient(client.getNickname(), msg);
        return;
    }
    std::string user = inviteParams[0];
    std::string channelName = inviteParams[1];
    Channel* channel = server.getChannel(channelName);
    std::string errorMsg;
    if (!channel)
    {
        errorMsg = ":" + IRCname + " " + IRCerror::ERR_NOCHANMODES + " " + \
                            client.getNickname() + " " + channelName + \
                            " :Channel does not exist";
        server.sendToClient(client.getNickname(), errorMsg);
        return;
    }
    if (!(channel->getIsInviteOnly()))
    {
        errorMsg = "NOTICE " + client.getNickname() + " :Channel is not invite-only";
        server.sendToClient(client.getNickname(), errorMsg);
        return;
    }
    if (!isOperatorOnChannel(client, server, channel))
		return;
    if (isUserOnChannel(client, server, channel, user))
        return;
    if (isInvitedUserOnChannel(client, server, channel, user))
        return;
    processInviteRequest(client, server, channel, user);
}

void ChannelCmds::topicUserCmd(IClient& client, IServer& server, \
    std::vector<std::string>& topicParams)
{
    if (topicParams.empty())
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS + \
            " " + client.getNickname() + " TOPIC :Not enough parameters";
        server.sendToClient(client.getNickname(), errorMsg);
        return;
    }
    std::string channelName = topicParams[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NOSUCHCHANNEL + \
            " " + client.getNickname() + " " + channelName + " :No such channel";
        server.sendToClient(client.getNickname(), errorMsg);
        return;
    }
    if (!canOnlyOperatorChangeTopic(client, server, channel))
        return;
    if (topicParams.size() == 1)
        processGetTopicRequest(client, server, channel);
    std::string newTopic = topicParams[1];
    if (topicParams.size() == 2 && newTopic.front() == ':')
    {
        newTopic.erase(0, 1);
        processSetTopicRequest(client, server, channel, newTopic);
    }
}

void ChannelCmds::modeChannelCmd(IClient& client, IServer& server, \
    std::vector<std::string>& modeParams)
{
    std::string channelName = modeParams[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        sendNoChannelReply(client, server, channelName);
        return;
    }
    if (modeParams.size() == 1)
    {
        processModeOneArgsRequest(client, server, channel);
        return;
    }
    
    if (!isOperatorOnChannel(client, server, channel))
        return;
    if (modeParams.size() >= 2) {
        std::string mode = modeParams[1];
        if (modeParams.size() == 2)
            processModeTwoArgsRequest(client, server, channel, mode);
        else if (modeParams.size() == 3)
            processModeThreeArgsRequest(client, server, channel, mode, modeParams[2]);
    }
}
