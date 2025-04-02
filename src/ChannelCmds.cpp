#include <vector>
#include <iostream>
#include <string>

#include "../include/errorReplies.hpp"
#include "../include/Channel.hpp"
#include "../include/ChannelCmds.hpp"
#include "../include/channelHelperFcts.hpp"

#include "../include/IServer.hpp"
#include "../include/IChannel.hpp"
#include "../include/IClient.hpp"
#include "../include/Client.hpp"
#include "../include/client_c_h.hpp"

void ChannelCmds::joinChannelCmd(IClient& client, IServer& server, std::vector<std::string>& joinParams)
{
    IChannel* channel;
    std::vector<std::string> channelNames;
    std::vector<std::string> passwords;

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
        if (Channel::isValidChannelName(channelNames[i]))
        {
            channel = server.getChannel(channelNames[i]);
            if (!channel)
            {
                channel = server.createChannel(channelNames[i]);
                channel->addOperator(client.getNickname());
            }
        }
        else
        {
            std::cerr << "Error code " << ERR_BADCHANMASK << ": bad channel mask. Invalid channel name: " << channelNames[i] << std::endl;
            continue;
        }
        if (channel->getHasPassword() && channel->getPassword() != passwords[i])
        {
            CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_BADCHANNELKEY) + ": invalid password.");
            continue;
        }
        bool isInvited = std::find(channel->getInvitedUsers().begin(), channel->getInvitedUsers().end(), client.getNickname()) != channel->getInvitedUsers().end();
        if (channel->getIsInviteOnly() && !isInvited)
        {
            CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_INVITEONLYCHAN) + ":Cannot join. Channel is invite only");
            continue;
        }
        if (channel->getUserLimit() > 0 && channel->getUsers().size() >= channel->getUserLimit())
        {
            CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_CHANNELISFULL) + ":Cannot join. Channel is full");
            continue;
        }
        channel->addUser(client.getNickname());
        CommandHandler::SendMessage(&client, client.getNickname() + " JOIN " + channelNames[i]);
    }
}

void ChannelCmds::partChannelCmd(IClient& client, IServer& server, std::vector<std::string>& partParams)
{
    IChannel* channel;
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
        channel = server.getChannel(channelNames[i]);
        if (!channel)
        {
            CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_NOSUCHCHANNEL) + " :non existing channel " + channelNames[i]);
            continue;
        }

        bool isClientOnChannel = std::find(channel->getUsers().begin(), channel->getUsers().end(), client.getNickname()) != channel->getUsers().end();
        if (!isClientOnChannel)
        {
            CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_NOTONCHANNEL) + " :you are not on channel " + channelNames[i]);
            continue;
        }
        channel->removeUser(client.getNickname());
        if (reason.empty())
            CommandHandler::SendMessage(&client, client.getNickname() + " PART " + channelNames[i]);
        else
            CommandHandler::SendMessage(&client, client.getNickname() + " PART " + channelNames[i] + ":" + reason);
        if (channel->getUsers().size() == 0)
            server.removeChannel(channelNames[i]);
    }
}

void ChannelCmds::kickUserCmd(IClient& client, IServer& server, std::vector<std::string>& kickParams)
{
    IChannel* channel;
    std::vector<std::string> channelNames;
    std::vector<std::string> userNames;
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
		channel = server.getChannel(channelNames[i]);
        if (!channel)
        {
			CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_NOSUCHCHANNEL) + " :non existing channel " + channelNames[i]);
            continue;
        }
		std::vector<std::string> operators = channel->getOperators();
        bool isOperator = std::find(operators.begin(), operators.end(), client.getNickname()) != operators.end();
        if (!isOperator || channel->getOperators().empty())
        {
			CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_CHANOPRIVSNEEDED) + ": you are not a channel operator of " + channelNames[i]);
            continue;
        }
        bool isClientOnChannel = std::find(channel->getUsers().begin(), channel->getUsers().end(), client.getNickname()) != channel->getUsers().end();
        if (!isClientOnChannel || channel->getUsers().empty())
        {
			CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_NOTONCHANNEL) + ": you are not on channel " + channelNames[i]);
            continue;
        }
		std::vector<std::string> usersInChannel = channel->getUsers();
		bool isUserOnChannel = std::find(usersInChannel.begin(), usersInChannel.end(), userNames[i]) != usersInChannel.end();
        if (!isUserOnChannel || channel->getUsers().empty())
        {
			CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_USERNOTINCHANNEL) + ": can not kick. user " + userNames[i] + " is not in channel " + channelNames[i]);
            continue;
        }
        channel->removeUser(userNames[i]);
        if (reason.empty())
            CommandHandler::SendMessage(&client, client.getNickname() + " KICK " + userNames[i] + " from " + channelNames[i]);
        else
            CommandHandler::SendMessage(&client, client.getNickname() + " KICK " + userNames[i] + " from " + channelNames[i] + " :" + reason);
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
    if (!channel->isOperator(client.getNickname()) || channel->getOperators().empty())
    {
        CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_CHANOPRIVSNEEDED) + ": can not set TOPIC. you are not a channel operator of " + channelName);
        return;
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