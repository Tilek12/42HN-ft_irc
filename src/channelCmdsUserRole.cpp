#include "../include/errorReplies.hpp"
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
				std::string(ERR_CHANOPRIVSNEEDED) + \
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
			std::string(ERR_NOTONCHANNEL) + ": you are not on channel " + channel->getName());
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
			std::string(ERR_USERNOTINCHANNEL) + ": can not kick. user " + userName + \
			" is not in channel " + channel->getName());
		return false;
	}
	else
		return true;
}
