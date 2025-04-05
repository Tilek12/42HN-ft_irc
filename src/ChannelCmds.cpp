#include <vector>
#include <iostream>
#include <string>

#include "../include/errorReplies.hpp"
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
        std::cerr << "Error code " << ERR_NEEDMOREPARAMS << " JOIN: not enough joinParams" << std::endl;
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
        std::cerr << "Error code " << ERR_NEEDMOREPARAMS << " PART: not enough partParams" << std::endl;
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
        std::cerr << "Error code " << ERR_NEEDMOREPARAMS << " KICK: not enough kickParams" << std::endl;
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
        std::cerr << "Error code " << ERR_NEEDMOREPARAMS << " INVITE: not enough inviteParams" << std::endl;
        return;
    }
    std::string user = inviteParams[0];
    std::string channelName = inviteParams[1];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_NOSUCHCHANNEL) + " : non existing channel " + channelName);
        return;
    }
    if (!(channel->getIsInviteOnly()))
    {
        CommandHandler::SendMessage(&client, "Error: no isInvitedOnly channel " + channelName);
        return;
    }
    if (!channel->isOperator(client.getNickname()) || channel->getOperators().empty())
    {
        CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_CHANOPRIVSNEEDED) + ": can not invite. you are not a channel operator of " + channelName);
        return;
    }
    if (channel->isUser(user))
    {
        CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_USERONCHANNEL) + ": user " + user + " already on channel " + channelName);
        return;
    }
    if (channel->isInvitedUser(user))
    {
        CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_USERONCHANNEL) +  ": user " + user +  " already in invitedUsers on channel " + channelName);
        return;
    }
    channel->addInvitedUser(user);
    CommandHandler::SendMessage(&client, client.getNickname() + " INVITE " + user + " to " + channelName);
}

void ChannelCmds::topicUserCmd(IClient& client, IServer& server, std::vector<std::string>& topicParams)
{
    if (topicParams.empty())
    {
        std::cerr << "Error code " << ERR_NEEDMOREPARAMS << " TOPIC: not enough topicParams" << std::endl;
        return;
    }
    std::string channelName = topicParams[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_NOSUCHCHANNEL) + " : non existing channel " + channelName);
        return;
    }
    if (channel->getOnlyOperatorCanChangeTopic())
    {
        if (!channel->isOperator(client.getNickname()) || channel->getOperators().empty())
        {
            CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_CHANOPRIVSNEEDED) + ": can not set TOPIC. you are not a channel operator of " + channelName);
            return;
        }
    }
    if (topicParams.size() == 1)
    {
        if (channel->getTopic().empty())
            CommandHandler::SendMessage(&client, "Reply code " + std::string(RPL_NOTOPIC) + " : no TOPIC existing in channel " + channelName);
        else
            CommandHandler::SendMessage(&client, "Reply code " + std::string(RPL_TOPIC) + " : TOPIC of channel " + channelName + " is " + channel->getTopic());
        return;
    }
    std::string newTopic = topicParams[1];
    if (topicParams.size() == 2 && newTopic.front() == ':')
    {
        channel->setTopic(newTopic.erase(0, 1));
        if (channel->getTopic().empty())
            CommandHandler::SendMessage(&client, "Reply code " + std::string(RPL_NOTOPIC) + " : no TOPIC existing in channel " + channelName);
        else
            CommandHandler::SendMessage(&client, "TOPIC of channel " + channelName + " is: " + channel->getTopic());
        return;
    }
}

void ChannelCmds::modeChannelCmd(IClient& client, IServer& server, std::vector<std::string>& modeParams)
{
    if (modeParams.size() < 2)
    {
        std::cerr << "Error code " << ERR_NEEDMOREPARAMS << " MODE: not enough modeParams" << std::endl;
        return;
    }
    std::string channelName = modeParams[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_NOCHANMODES) + " : non existing channel " + channelName);
        return;
    }
    if (!channel->isOperator(client.getNickname()) || channel->getOperators().empty())
    {
        CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_CHANOPRIVSNEEDED) + ": can not MODE. you are not a channel operator of " + channelName);
        return;
    }
    std::string mode = modeParams[1];
    if (modeParams.size() == 2)
    {
        if (mode == "+i")
        {
            channel->setIsInviteOnly(true);
            return;
        }
        else if (mode == "-i")
        {
            channel->setIsInviteOnly(false);
            return;
        }
        else if (mode == "+t")
        {
            channel->setOnlyOperatorCanChangeTopic(true);
            return;
        }
        else if (mode == "-t")
        {
            channel->setOnlyOperatorCanChangeTopic(false);
            return;
        }
        else if (mode == "-l")
        {
            channel->setUserLimit(0);
            return;
        }
        else
        {
            CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_UNKNOWNMODE) + ": unknown channel mode " + mode);
            return;
        }
    }
    else if (modeParams.size() == 3)
    {
        if (mode == "+o" || mode == "-o")
        {
            std::string user = modeParams[2];
            if (mode == "+o")
                channel->addOperator(user);
            else if (mode == "-o")
                channel->removeOperator(user);
        }
        else if (mode == "+k" || mode == "-k")
        {
            std::string password = modeParams[2];
            if (mode == "+k")
            {
                channel->setHasPassword(true);
                channel->setPassword(password);
            }
            else if (mode == "-k")
            {
                channel->setHasPassword(false);
                channel->setPassword("");
            }
        }
        else if (mode == "+l")
        {
            int userLimit = std::stoi(modeParams[2]);
            channel->setUserLimit(userLimit);
        }
        else
        {
            CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_UNKNOWNMODE) + ": unknown channel mode " + mode);
            return;
        }
    }
}