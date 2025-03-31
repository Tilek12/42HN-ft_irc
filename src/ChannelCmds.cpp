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
			CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_CHANOPRIVSNEEDED) + ":you are not a channel operator of " + channelNames[i]);
            continue;
        }
        bool isClientOnChannel = std::find(channel->getUsers().begin(), channel->getUsers().end(), client.getNickname()) != channel->getUsers().end();
        if (!isClientOnChannel || channel->getUsers().empty())
        {
			CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_NOTONCHANNEL) + ":you are not on channel " + channelNames[i]);
            continue;
        }
		std::vector<std::string> usersInChannel = channel->getUsers();
		bool isUserOnChannel = std::find(usersInChannel.begin(), usersInChannel.end(), userNames[i]) != usersInChannel.end();
        if (!isUserOnChannel || channel->getUsers().empty())
        {
			CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_USERNOTINCHANNEL) + ":user " + userNames[i] + " is not in channel " + channelNames[i]);
            continue;
        }
        channel->removeUser(userNames[i]);
        if (reason.empty())
            CommandHandler::SendMessage(&client, client.getNickname() + " KICK " + userNames[i] + " from " + channelNames[i]);
        else
            CommandHandler::SendMessage(&client, client.getNickname() + " KICK " + userNames[i] + " from " + channelNames[i] + " :" + reason);
    }
}
