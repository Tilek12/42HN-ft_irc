#include <vector>
#include <iostream>

#include "../include/errorReplies.hpp"
#include "../include/Channel.hpp"
#include "../include/Command.hpp"
#include "../include/commandHelperFtcs.hpp"

#include "IClient.hpp"
#include "../include/IServer.hpp"

static void joinChannelCmd(IClient& client, IServer& server, std::vector<std::string>& joinParams)
{
	Channel* channel;
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
		if (isValidChannelName(channelNames[i]))
		{
			channel = server.getChannel(channelNames[i]);
			if (!channel)
			{
				channel = server.createChannel(channelName[i]);
				channel->addOperator(client.getNickname());
			}
		}
		if (channel->getHasPassword() && channel->getPassword() != passwords[i])
		{
			client.SendMessage("Error code " << ERR_BADCHANNELKEY << ": invalid password.");
			continue;
		}
		if (channel->getIsInviteOnly() && \
			channel->getInvitedUsers().find(client.getNickname) == channel->getInvitedUsers().end())
		{
			client.SendMessage("Error code " << ERR_INVITEONLYCHAN << ":Cannot join. Channel is invite only");
			continue;
		}
		if (channel->getUserLimit() > 0 && channel->getUsers().size() >= channel->getUserLimit())
		{
			client.SendMessage("Error code " << ERR_CHANNELISFULL << ":Cannot join. Channel is full");
			continue;
		}
		channel->addUser(client.getNickname());
		client.sendMessage("JOIN " + channelName[i]);
	}

}
