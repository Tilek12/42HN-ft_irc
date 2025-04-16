#include "../include/IRCreply.hpp"
#include "../include/IRCerror.hpp"
#include "../include/IClient.hpp"
#include "../include/IChannel.hpp"
#include "../include/client_c_h.hpp"

bool isOperatorOnChannel(IClient& client, IChannel* channel)
{
	std::vector<std::string> operators = channel->getOperators();
	bool isOperator = std::find(operators.begin(), operators.end(), \
		client.getNickname()) != operators.end();
	if (!isOperator || operators.empty())
	{
		CommandHandler::SendMessage(&client, "Error code " + \
				std::string(IRCerror::ERR_CHANOPRIVSNEEDED) + \
				": you are not a channel operator of " + channel->getName());
		return false;
	}
	else
		return true;
}

bool isClientOnChannel(IClient& client, IChannel* channel)
{
	bool isClientOnChannel = std::find(channel->getUsers().begin(), \
		channel->getUsers().end(), client.getNickname()) != channel->getUsers().end();
	if (!isClientOnChannel || channel->getUsers().empty())
	{
		CommandHandler::SendMessage(&client, "Error code " + \
			std::string(IRCerror::ERR_NOTONCHANNEL) + ": you are not on channel " + channel->getName());
		return false;
	}
	else
		return true;
}

bool isUserOnChannel(IClient& client, IChannel* channel, const std::string& userName)
{
	std::vector<std::string> usersInChannel = channel->getUsers();
	bool isUserOnChannel = std::find(usersInChannel.begin(), \
		usersInChannel.end(), userName) != usersInChannel.end();
	if (!isUserOnChannel || usersInChannel.empty())
	{
		CommandHandler::SendMessage(&client, "Error code " + \
			std::string(IRCerror::ERR_USERNOTINCHANNEL) + " " + userName + " is not in channel " \
			+ channel->getName());
		return false;
	}
	else
		return true;
}

bool isInvitedUserOnChannel(IClient& client, IChannel* channel, const std::string& userName)
{
	if (channel->isInvitedUser(userName))
	{
		CommandHandler::SendMessage(&client, "Error code " + \
			std::string(IRCerror::ERR_USERONCHANNEL) +  ": user " + userName +  \
			" already in invitedUsers on channel " + channel->getName());
		return true;
	}
	else
		return false;
}

void canOnlyOperatorChangeTopic(IClient& client, IChannel* channel)
{
	if (channel->getOnlyOperatorCanChangeTopic())
    {
        if (!channel->isOperator(client.getNickname()) || channel->getOperators().empty())
        {
            CommandHandler::SendMessage(&client, "Error code " + \
                std::string(IRCerror::ERR_CHANOPRIVSNEEDED) + ": can not set TOPIC. "\
                "You are not a channel operator of " + channel->getName());
            return;
        }
    }
	return;
}
