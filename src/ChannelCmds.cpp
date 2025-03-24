#include <vector>
#include <iostream>
#include <string>

#include "../include/errorReplies.hpp"
#include "../include/Channel.hpp"
#include "../include/ChannelCmds.hpp"
#include "../include/channelHelperFcts.hpp"

#include "../include/IServer.hpp"
#include "../include/IChannel.hpp"
#include "client/IClient.hpp"

static void joinChannelCmd(IClient& client, IServer& server, std::vector<std::string>& joinParams)
{
	IChannel* channel;
	std::vector<std::string> channelNames;
	std::vector<std::string> passwords;

	if (joinParams.empty())
	{
		std::cerr << "Error code " << ERR_NEEDMOREPARAMS << \
			" JOIN: not enough joinParams" << std::endl;
		return ;
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
		if (channel->getHasPassword() && channel->getPassword() != passwords[i])
		{
			client.SendMessage(&client, "Error code " + std::string(ERR_BADCHANNELKEY) + ": invalid password.");
			continue;
		}
		if (channel->getIsInviteOnly() && \
			channel->getInvitedUsers().find(client.getNickname()) == channel->getInvitedUsers().end())
		{
			client.SendMessage(&client, "Error code " + std::string(ERR_INVITEONLYCHAN) + ":Cannot join. Channel is invite only");
			continue;
		}
		if (channel->getUserLimit() > 0 && channel->getUsers().size() >= channel->getUserLimit())
		{
			client.SendMessage(&client, "Error code " + std::string(ERR_CHANNELISFULL) + ":Cannot join. Channel is full");
			continue;
		}
		channel->addUser(client.getNickname());
		client.SendMessage(&client, "JOIN " + channelNames[i]);
	}

}
