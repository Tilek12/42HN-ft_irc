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
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_BADCHANMASK + " " \
            + client.getNickname() + " " + channelName + " :Bad Channel Mask";
        server.sendToClient(client.getNickname(), errorMsg);
        return;
    }
    if (!joinAllowed(client, server, channel, password))
        return;
    channel->addUser(client.getNickname());
    std::string joinMsg = ":" +  client.getNickname() + "!" +
                          "@" + client.getHostname() + " JOIN :" + channel->getName();
    server.sendToClient(client.getNickname(), joinMsg);
    server.broadcastMessage(realClient, channel->getName(), joinMsg);

    if (channel->isOperator(client.getNickname())) {
        std::string modeMsg = ":" + client.getNickname() + " MODE " + channel->getName() + \
            " +o " + client.getNickname();
        server.broadcastMessage(realClient, channel->getName(), modeMsg);
        server.sendToClient(client.getNickname(), modeMsg);
    }
    sendNameReplies(realClient, channel, server);
}

bool joinAllowed(IClient& client, IServer& server, IChannel* channel, const std::string& password)
{
    if (channel->getHasPassword() && !channel->isValidPassword(password))
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_BADCHANNELKEY \
            + " " + client.getNickname() + " " + channel->getName() + " :Cannot join channel (+k)";
        server.sendToClient(client.getNickname(), errorMsg);
        return false;
    }
    bool isInvited = std::find(channel->getInvitedUsers().begin(), \
		channel->getInvitedUsers().end(), client.getNickname()) \
        != channel->getInvitedUsers().end();
    if (channel->getIsInviteOnly() && !isInvited)
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_INVITEONLYCHAN +
        " " + client.getNickname() + " " + channel->getName() + " :Cannot join channel (+i)";
        server.sendToClient(client.getNickname(), errorMsg);
        return false;
    }
    if (channel->getUserLimit() > 0 && channel->getUsers().size() >= channel->getUserLimit())
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_CHANNELISFULL +
            " " + client.getNickname() + " " + channel->getName() + " :Cannot join channel (+l)";
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
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NOSUCHCHANNEL +
            " " + client.getNickname() + " " + channelName + " :No such channel\r\n";
        server.sendToClient(client.getNickname(), errorMsg);
        std::cerr << errorMsg;
        return;
    }
    bool isClientOnChannel = std::find(channel->getUsers().begin(), \
		channel->getUsers().end(), client.getNickname()) != channel->getUsers().end();
    if (!isClientOnChannel)
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NOTONCHANNEL +
            " " + client.getNickname() + " " + channelName + " :You're not on that channel\r\n";
        server.sendToClient(client.getNickname(), errorMsg);
        std::cerr << errorMsg;
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
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NOSUCHCHANNEL + " " +
            client.getNickname() + " " + channelName + " :No such channel";
        server.sendToClient(client.getNickname(), errorMsg);
        server.broadcastMessage(realClient, client.getNickname(), errorMsg);
        std::cerr << errorMsg << std::endl;
		return;
	}
	if (!isOperatorOnChannel(client, server, channel))
		return;
	if (!isClientOnChannel(client, server, channel))
		return;
    if (!isUserOnChannel(client, server, channel, userName))
		return;

    server.removeChannel(client.getNickname());
    std::string reply;
    if (reason.empty())
        reply = ":" + client.getNickname() + " KICK " + channelName + " " + userName + "\r\n";
    else
        reply = ":" + client.getNickname() + " KICK " + channelName + " " + userName + " :" + reason + "\r\n";

    server.broadcastMessage(realClient, channel->getName(), reply);
    server.sendToClient(client.getNickname(), reply);
    if (channel->getUsers().size() == 0)
        server.removeChannel(channelName);
}

void processInviteRequest(IClient& client, IServer& server, IChannel* channel, \
    const std::string& userName)
{
    channel->addInvitedUser(userName);
    std::string reply = ":" + client.getNickname() + " INVITE " +
        userName + " " + channel->getName();
    server.sendToClient(userName, reply);
}

void processGetTopicRequest(IClient& client, IServer& server, IChannel* channel)
{
    Client* realClient = dynamic_cast<Client*>(&client);
    if (channel->getTopic().empty())
    {
        std::string reply = ":" + IRCname + " " + IRCreply::RPL_NOTOPIC + " " +
                client.getNickname() + " " + channel->getName() + " :No topic is set for this channel";
        server.sendToClient(client.getNickname(), reply);
        std::cerr << reply << std::endl;
    }
    else
    {
        std::string reply = ":" + IRCname + " " + IRCreply::RPL_TOPIC + " " +
                client.getNickname() + " " + channel->getName() + " :" + channel->getTopic();
        server.sendToClient(client.getNickname(), reply);
    }
    return;
}

void processSetTopicRequest(IClient& client, IServer& server, IChannel* channel, std::string newTopic)
{
    Client* realClient = dynamic_cast<Client*>(&client);

    std::string reply;
    channel->setTopic(newTopic);
    if (channel->getTopic().empty())
        reply = ":" + IRCname + " " + IRCreply::RPL_NOTOPIC + " " + \
        client.getNickname() + " " + channel->getName() + " :No topic is set for this channel";
    else
        reply = ":" + IRCname + " " + IRCreply::RPL_TOPIC + " " + \
                    client.getNickname() + " " + channel->getName() + " :" + channel->getTopic();
    server.sendToClient(client.getNickname(), reply);
    server.broadcastMessage(realClient, channel->getName(), reply);
}

void processModeTwoArgsRequest(IClient& client, IServer& server, IChannel* channel, std::string mode)
{
    if (mode == "b")
    {
        std::string reply = ":" + IRCname + IRCreply::RPL_ENDOFBANLIST + " " + \
            client.getNickname() + " " + channel->getName() + " :End of Channel Ban List\r\n";
        server.sendToClient(client.getNickname(), reply);
    }
    else if (mode == "+i")
        channel->setIsInviteOnly(true);
    else if (mode == "-i")
        channel->setIsInviteOnly(false);
    else if (mode == "+t")
    {
        channel->setOnlyOperatorCanChangeTopic(true);
std::cout << "debug bool OnlyOpCanChangeTopic: " << channel->getOnlyOperatorCanChangeTopic() << std::endl; 
    }
    else if (mode == "-t")
    {
        channel->setOnlyOperatorCanChangeTopic(false);
std::cout << "debug bool OnlyOpCanChangeTopic: " << channel->getOnlyOperatorCanChangeTopic() << std::endl; 

    }
    else if (mode == "-l")
        channel->setUserLimit(0);
    else
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_UNKNOWNMODE + " " +
            client.getNickname() + " " + mode + " :Unknown channel mode";
        server.sendToClient(client.getNickname(), errorMsg);
        std::cerr << errorMsg << std::endl;
    }
    return;
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
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_UNKNOWNMODE + " " +
            client.getNickname() + " " + mode + " :Unknown channel mode";
        server.sendToClient(client.getNickname(), errorMsg);
        std::cerr << errorMsg << std::endl;
        validMode = false;
    }
    if (validMode) 
    {
        std::string reply = ":" + client.getNickname() + " MODE " + channel->getName() + " " + mode + " " + modeParamIdx2;
        server.broadcastMessage(dynamic_cast<Client*>(&client), channel->getName(), reply);
        server.sendToClient(client.getNickname(), reply);
    }
}
