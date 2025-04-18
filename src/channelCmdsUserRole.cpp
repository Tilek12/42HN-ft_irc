#include "../include/IRCerror.hpp"
#include "../include/IClient.hpp"
#include "../include/IChannel.hpp"
#include "../include/client_c_h.hpp"

bool isOperatorOnChannel(IClient& client, IServer& server, IChannel* channel)
{
	std::vector<std::string> operators = channel->getOperators();
	bool isOperator = std::find(operators.begin(), operators.end(), \
		client.getNickname()) != operators.end();
	if (!isOperator || operators.empty())
	{
		std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_CHANOPRIVSNEEDED + " " +
			client.getNickname() + " " + channel->getName() + " :You are not a channel operator";
		server.sendToClient(client.getNickname(), errorMsg);
		return false;
	}
	else
		return true;
}

bool isClientOnChannel(IClient& client, IServer& server, IChannel* channel)
{
	bool isClientOnChannel = std::find(channel->getUsers().begin(), \
		channel->getUsers().end(), client.getNickname()) != channel->getUsers().end();
	if (!isClientOnChannel || channel->getUsers().empty())
	{
		std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NOTONCHANNEL + " " +
			client.getNickname() + " " + channel->getName() + " :You are not on channel";
		server.sendToClient(client.getNickname(), errorMsg);
		return false;
	}
	else
		return true;
}

bool isUserOnChannel(IClient& client, IServer& server, IChannel* channel, const std::string& userName)
{
	std::vector<std::string> usersInChannel = channel->getUsers();
	bool isUserOnChannel = std::find(usersInChannel.begin(), \
		usersInChannel.end(), userName) != usersInChannel.end();
	if (!isUserOnChannel || usersInChannel.empty())
	{
		std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_USERNOTINCHANNEL + " " +
			client.getNickname() + " " + userName + " " + channel->getName() +
			" :User is not in channel";
		server.sendToClient(client.getNickname(), errorMsg);
		return false;
	}
	else
		return true;
}

bool isInvitedUserOnChannel(IClient& client, IServer& server, IChannel* channel, const std::string& userName)
{
	if (channel->isInvitedUser(userName))
	{
		std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_USERONCHANNEL + " " +
			client.getNickname() + " " + userName + " " + channel->getName() + \
			" :User is already in invitedUsers list for channel";
		server.sendToClient(client.getNickname(), errorMsg);
		return true;
	}
	else
		return false;
}

bool canOnlyOperatorChangeTopic(IClient& client, IServer& server, IChannel* channel)
{
	if (channel->getOnlyOperatorCanChangeTopic())
    {
        if (!channel->isOperator(client.getNickname()) || channel->getOperators().empty())
        {
			std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_CHANOPRIVSNEEDED + " " +
           		client.getNickname() + " " + channel->getName() +
            	" :Cannot set TOPIC. You are not a channel operator";
			server.sendToClient(client.getNickname(), errorMsg);
            return false;
        }
    }
	return true;
}
