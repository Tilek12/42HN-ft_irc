#include "../include/ChannelCmds.hpp"
#include "../include/errorReplies.hpp"
#include "../include/client_c_h.hpp"
#include "../include/channelHelperFcts.hpp"

void processJoinRequest(IClient& client, IServer& server, \
    const std::string& channelName, const std::string& password)
{
    IChannel* channel;
    if (Channel::isValidChannelName(channelName))
    {
        channel = server.getChannel(channelName);
        if (!channel)
        {
            channel = server.createChannel(channelName);
            channel->addOperator(client.getNickname());
        }
    }
    else
    {
        std::cerr << "Error code " << ERR_BADCHANMASK << ": bad channel mask. \
            Invalid channel name: " << channelName << std::endl;
        return;
    }
    if (!joinAllowed(client, channel, password))
    {
        return;
    }
    channel->addUser(client.getNickname());
    // CommandHandler::SendMessage(&client, client.getNickname() + " JOIN " + channelName);
    server.sendToClient(client.getNickname(), client.getNickname() + " JOIN " + channelName);
}

bool joinAllowed(IClient& client, IChannel* channel, const std::string& password)
{
    if (channel->getHasPassword() && channel->isValidPassword(password))
    {
        CommandHandler::SendMessage(&client, "Error code " + \
			std::string(ERR_BADCHANNELKEY) + ": invalid password.");
        return false;
    }
    bool isInvited = std::find(channel->getInvitedUsers().begin(), \
		channel->getInvitedUsers().end(), client.getNickname()) \
        != channel->getInvitedUsers().end();
    if (channel->getIsInviteOnly() && !isInvited)
    {
        CommandHandler::SendMessage(&client, "Error code " + std::string(ERR_INVITEONLYCHAN) + \
            ":Cannot join. Channel is invite only");
        return false;
    }
    if (channel->getUserLimit() > 0 && channel->getUsers().size() >= channel->getUserLimit())
    {
        CommandHandler::SendMessage(&client, "Error code " + \
			std::string(ERR_CHANNELISFULL) + ":Cannot join. Channel is full");
        return false;
    }
    return true;
}

void processPartRequest(IClient& client, IServer& server, \
    const std::string& channelName, const std::string& reason)
{
    IChannel* channel;
    
    channel = server.getChannel(channelName);
    if (!channel)
    {
        CommandHandler::SendMessage(&client, "Error code " + \
			std::string(ERR_NOSUCHCHANNEL) + " :non existing channel " + channelName);
        return;
    }
    bool isClientOnChannel = std::find(channel->getUsers().begin(), \
		channel->getUsers().end(), client.getNickname()) != channel->getUsers().end();
    if (!isClientOnChannel)
    {
        CommandHandler::SendMessage(&client, "Error code " + \
			std::string(ERR_NOTONCHANNEL) + " :you are not on channel " + channelName);
        return;
    }
    channel->removeUser(client.getNickname());
    if (reason.empty())
        CommandHandler::SendMessage(&client, client.getNickname() + " PART " + channelName);
    else
        CommandHandler::SendMessage(&client, client.getNickname() + " PART " + \
			channelName + ":" + reason);
    if (channel->getUsers().size() == 0)
        server.removeChannel(channelName);
}

void processKickRequest(IClient& client, IServer& server, \
    const std::string& channelName, const std::string& userName, const std::string& reason)
{
	IChannel* channel;
	
	channel = server.getChannel(channelName);
	if (!channel)
	{
		CommandHandler::SendMessage(&client, "Error code " + \
			std::string(ERR_NOSUCHCHANNEL) + " :non existing channel " + channelName);
		return;
	}
	if (!isOperatorOnChannel(client, channel))
		return;
	if (!isClientOnChannel(client, channel))
		return;
    if (!isUserOnChannel(client, channel, userName))
		return;
	channel->removeUser(userName);
	if (reason.empty())
		CommandHandler::SendMessage(&client, client.getNickname() + " KICK " + \
            userName + " from " + channelName);
	else
		CommandHandler::SendMessage(&client, client.getNickname() + " KICK " + \
            userName + " from " + channelName + " :" + reason);
}

void processInviteRequest(IClient& client, IChannel* channel, \
    const std::string& userName)
{
    channel->addInvitedUser(userName);
    CommandHandler::SendMessage(&client, client.getNickname() + " INVITE " + \
        userName + " to " + channel->getName());
}

void processGetTopicRequest(IClient& client, IChannel* channel)
{
    if (channel->getTopic().empty())
            CommandHandler::SendMessage(&client, "Reply code " + \
                std::string(RPL_NOTOPIC) + " : no TOPIC existing in channel " \
                + channel->getName());
    else
        CommandHandler::SendMessage(&client, "Reply code " + \
            std::string(RPL_TOPIC) + " : TOPIC of channel " + channel->getName() + \
            " is " + channel->getTopic());
    return;
}

void processSetTopicRequest(IClient& client, IChannel* channel, std::string newTopic)
{
    channel->setTopic(newTopic.erase(0, 1));
    if (channel->getTopic().empty())
        CommandHandler::SendMessage(&client, "Reply code " + \
            std::string(RPL_NOTOPIC) + " : no TOPIC existing in channel " + \
            channel->getName());
    else
        CommandHandler::SendMessage(&client, "TOPIC of channel " + \
            channel->getName() + " is: " + channel->getTopic());
    return;
}

void processModeTwoArgsRequest(IClient& client, IChannel* channel, std::string mode)
{
    if (mode == "+i")
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
        CommandHandler::SendMessage(&client, "Error code " + \
            std::string(ERR_UNKNOWNMODE) + ": unknown channel mode " + mode);
    }
    return;
}
void processModeThreeArgsRequest(IClient& client, IChannel* channel, std::string mode, std::string modeParamIdx2)
{
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
        CommandHandler::SendMessage(&client, "Error code " + \
            std::string(ERR_UNKNOWNMODE) + ": unknown channel mode " + mode);
        return;
    }
}
