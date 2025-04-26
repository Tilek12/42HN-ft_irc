/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelCmdsUtils.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llacsivy <llacsivy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:46:14 by llacsivy          #+#    #+#             */
/*   Updated: 2025/04/26 17:46:17 by llacsivy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ChannelCmds.hpp"
#include "../include/IRCerror.hpp"
#include "../include/IRCreply.hpp"
#include "../include/client_c_h.hpp"
#include "../include/channelHelperFcts.hpp"

void processJoinRequest(IClient& client, IServer& server, \
    const std::string& channelName, const std::string& password)
{
    IChannel* channel;
    Client* realClient = dynamic_cast<Client*>(&client);
    if (Channel::isValidChannelName(channelName))
    {
        channel = server.getChannel(channelName);
        if (!channel)
            handleNewChannel(server, channel, channelName, password, client);
    }
    else
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_BADCHANMASK + \
            " " + client.getNickname() + " " + channelName + " :Bad Channel Mask";
        server.sendToClient(client.getNickname(), errorMsg);
        return;
    }
    if (!joinAllowed(client, server, channel, password))
        return;
    channel->addUser(client.getNickname(), server);
    std::string joinMsg = ":" +  client.getNickname() + "!" + \
                          "@" + client.getHostname() + " JOIN :" + channel->getName();
    server.sendToClient(client.getNickname(), joinMsg);
    server.broadcastMessage(realClient, channel->getName(), joinMsg);

    if (channel->isOperator(client.getNickname())) {
        std::string modeMsg = ":" + client.getNickname() + " MODE " + \
            channel->getName() + " +o " + client.getNickname();
        server.broadcastMessage(realClient, channel->getName(), modeMsg);
        server.sendToClient(client.getNickname(), modeMsg);
    }
    sendNameReplies(realClient, channel, server);
}

bool joinAllowed(IClient& client, IServer& server, IChannel* channel, \
    const std::string& password)
{
    if (channel->getHasPassword() && !channel->isValidPassword(password))
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_BADCHANNELKEY \
            + " " + client.getNickname() + " " + channel->getName() + \
            " :Cannot join channel (+k)";
        server.sendToClient(client.getNickname(), errorMsg);
        return false;
    }
    bool isInvited = std::find(channel->getInvitedUsers().begin(), \
		channel->getInvitedUsers().end(), client.getNickname()) \
        != channel->getInvitedUsers().end();
    if (channel->getIsInviteOnly() && !isInvited)
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_INVITEONLYCHAN + \
        " " + client.getNickname() + " " + channel->getName() + \
        " :Cannot join channel (+i)";
        server.sendToClient(client.getNickname(), errorMsg);
        return false;
    }
    if (channel->getUserLimit() > 0 && \
        channel->getUsers().size() >= channel->getUserLimit())
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_CHANNELISFULL + \
            " " + client.getNickname() + " " + channel->getName() + \
            " :Cannot join channel (+l)";
        server.sendToClient(client.getNickname(), errorMsg);
        return false;
    }
    return true;
}

void processPartRequest(IClient& client, IServer& server, \
    const std::string& channelName, const std::string& reason)
{
    Client* realClient = dynamic_cast<Client*>(&client);
    IChannel* channel = server.getChannel(channelName);

    if (!channel)
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NOSUCHCHANNEL + \
            " " + client.getNickname() + " " + channelName + " :No such channel";
        server.sendToClient(client.getNickname(), errorMsg);
        return;
    }
    bool isClientOnChannel = std::find(channel->getUsers().begin(), \
		channel->getUsers().end(), client.getNickname()) != channel->getUsers().end();
    if (!isClientOnChannel)
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NOTONCHANNEL + \
            " " + client.getNickname() + " " + channelName + \
            " :You're not on that channel";
        server.sendToClient(client.getNickname(), errorMsg);
        return;
    }
    channel->removeUser(client.getNickname());
    std::string reply;
    if (reason.empty())
        reply = ":" + client.getNickname() + " PART " + channelName;
    else
        reply = ":" + client.getNickname() + " PART " + channelName + " :" + reason;
    server.sendToClient(client.getNickname(), reply);
    server.broadcastMessage(realClient, channel->getName(), reply);
    if (channel->getUsers().size() == 0)
        server.removeChannel(channelName);
}

void processKickRequest(IClient& client, IServer& server, \
    const std::string& channelName, const std::string& userName, const std::string& reason)
{
	IChannel* channel;
    Client* realClient = dynamic_cast<Client*>(&client);
	channel = server.getChannel(channelName);
	if (!channel)
	{
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NOSUCHCHANNEL + \
            " " + client.getNickname() + " " + channelName + " :No such channel";
        server.sendToClient(client.getNickname(), errorMsg);
        server.broadcastMessage(realClient, client.getNickname(), errorMsg);
		return;
	}
	if (!isOperatorOnChannel(client, server, channel))
		return;
	if (!isClientOnChannel(client, server, channel))
		return;
    if (!isUserOnChannel(client, server, channel, userName))
		return;
    channel->removeUser(userName);
    std::string reply;
    if (reason.empty())
        reply = ":" + client.getNickname() + " KICK " + channelName + \
            " " + userName;
    else
        reply = ":" + client.getNickname() + " KICK " + channelName + " " \
            + userName + " :" + reason;
    server.broadcastMessage(realClient, channel->getName(), reply);
    server.sendToClient(client.getNickname(), reply);
    server.sendToClient(userName, reply);
    if (channel->getUsers().size() == 0)
        server.removeChannel(channelName);
}

void processInviteRequest(IClient& client, IServer& server, IChannel* channel, \
    const std::string& userName)
{
    channel->addInvitedUser(userName);
    std::string reply = ":" + client.getNickname() + " INVITE " + \
        userName + " " + channel->getName();
    server.sendToClient(userName, reply);
}

void processGetTopicRequest(IClient& client, IServer& server, IChannel* channel)
{
    if (channel->getTopic().empty())
    {
        std::string reply = ":" + IRCname + " " + IRCreply::RPL_NOTOPIC + " " +
                client.getNickname() + " " + channel->getName() + \
                " :No topic is set for this channel";
        server.sendToClient(client.getNickname(), reply);
    }
    else
    {
        std::string reply = ":" + IRCname + " " + IRCreply::RPL_TOPIC + " " +
                client.getNickname() + " " + channel->getName() + " :" + \
                channel->getTopic();
        server.sendToClient(client.getNickname(), reply);
    }
    return;
}

void processSetTopicRequest(IClient& client, IServer& server, IChannel* channel, \
    std::string newTopic)
{
    Client* realClient = dynamic_cast<Client*>(&client);

    std::string reply;
    channel->setTopic(newTopic);
    if (channel->getTopic().empty())
        reply = ":" + IRCname + " " + IRCreply::RPL_NOTOPIC + " " + \
        client.getNickname() + " " + channel->getName() + \
        " :No topic is set for this channel";
    else
        reply = ":" + IRCname + " " + IRCreply::RPL_TOPIC + " " + \
            client.getNickname() + " " + channel->getName() + " :" + \
            channel->getTopic();
    server.sendToClient(client.getNickname(), reply);
    server.broadcastMessage(realClient, channel->getName(), reply);
}

void processModeTwoArgsRequest(IClient& client, IServer& server, \
    IChannel* channel, std::string mode)
{
    bool validMode = true;
    if (mode == "b")
    {
        std::string reply = ":" + IRCname + IRCreply::RPL_ENDOFBANLIST + " " + \
            client.getNickname() + " " + channel->getName() + \
            " :End of Channel Ban List";
        server.sendToClient(client.getNickname(), reply);
        validMode = false;
    }
    else if (mode == "+i")
        channel->setIsInviteOnly(true);
    else if (mode == "-i")
        channel->setIsInviteOnly(false);
    else if (mode == "+t")
        channel->setOnlyOperatorCanChangeTopic(true);
    else if (mode == "-t")
        channel->setOnlyOperatorCanChangeTopic(false);
    else if (mode == "-l")
        channel->setUserLimit(0);
    else
    {
        sendErrorMode(client, server, mode);
        validMode = false;
    }
    if (validMode) 
        sendModeTwoArgsMsg(client, server, channel, mode);
}

void processModeThreeArgsRequest(IClient& client, IServer& server, \
    IChannel* channel, std::string mode, std::string modeParamIdx2)
{
    bool validMode = true;
    if (mode == "+o")
        channel->addOperator(modeParamIdx2);
    else if (mode == "-o")
        channel->removeOperator(modeParamIdx2);
    else if (mode == "+k")
    {
        channel->setHasPassword(true);
        channel->setPassword(modeParamIdx2);
    }
    else if (mode == "-k")
    {
        channel->setHasPassword(false);
        channel->setPassword("");
    }
    else if (mode == "+l")
    {
        int userLimit = std::stoi(modeParamIdx2);
        channel->setUserLimit(userLimit);
    }
    else
    {
        sendErrorMode(client, server, mode);
        validMode = false;
    }
    if (validMode) 
        sendModeThreeArgsMsg(client, server, channel, mode, modeParamIdx2);
}
