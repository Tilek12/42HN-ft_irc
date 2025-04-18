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
        {
            channel = server.createChannel(channelName);
            channel->addOperator(client.getNickname());
        }
    }
    else
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_BADCHANMASK + " " \
            + client.getNickname() + " " + channelName + " :Bad Channel Mask\r\n";
        server.sendToClient(client.getNickname(), errorMsg);
        std::cerr << errorMsg;
        return;
    }
    if (!joinAllowed(client, server, channel, password))
        return;
    channel->addUser(client.getNickname());
    std::string joinMsg = ":" +  client.getNickname() + "!" +
                          "@" + client.getHostname() + " JOIN :" + channel->getName();
    server.sendToClient(client.getNickname(), joinMsg);
    server.broadcastMessage(realClient, channel->getName(), joinMsg);
    std::string userList;
    for (auto &&i : channel->getUsers())
        userList += i + " ";
    std::string reply353 = ":" + IRCname + " " + IRCreply::RPL_NAMREPLY + " " +
                           client.getNickname() + " = " + channel->getName() + " :" + userList + "\r\n";
    server.sendToClient(client.getNickname(), reply353);
    std::string reply366 = ":" + IRCname + " " + IRCreply::RPL_ENDOFNAMES + " " +
                           client.getNickname() + " " + channel->getName() + " :End of /NAMES list\r\n";
    server.sendToClient(client.getNickname(), reply366);
}

bool joinAllowed(IClient& client, IServer& server, IChannel* channel, const std::string& password)
{
    if (channel->getHasPassword() && channel->isValidPassword(password))
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_BADCHANNELKEY \
            + " " + client.getNickname() + " " + channel->getName() + " :Cannot join channel (+k)\r\n";
        server.sendToClient(client.getNickname(), errorMsg);
        std::cerr << errorMsg;
        return false;
    }
    bool isInvited = std::find(channel->getInvitedUsers().begin(), \
		channel->getInvitedUsers().end(), client.getNickname()) \
        != channel->getInvitedUsers().end();
    if (channel->getIsInviteOnly() && !isInvited)
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_INVITEONLYCHAN +
        " " + client.getNickname() + " " + channel->getName() + " :Cannot join channel (+i)\r\n";
        server.sendToClient(client.getNickname(), errorMsg);
        std::cerr << errorMsg;
        return false;
    }
    if (channel->getUserLimit() > 0 && channel->getUsers().size() >= channel->getUserLimit())
    {
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_CHANNELISFULL +
            " " + client.getNickname() + " " + channel->getName() + " :Cannot join channel (+l)\r\n";
        server.sendToClient(client.getNickname(), errorMsg);
        std::cerr << errorMsg;
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
    // std::cout << reply;
    // if (channel->getUsers().size() == 0)
    //     server.removeChannel(channelName);
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

// server.removeChannel(client.getNickname()); // This might need to be replaced by a function like removeUserFromChannel
// std::string reply;
// if (reason.empty())
//     reply = ":" + client.getNickname() + " KICK " + channelName + " " + userName + "\r\n";
// else
//     reply = ":" + client.getNickname() + " KICK " + channelName + " " + userName + " :" + reason + "\r\n";

// // Broadcast the message to all other clients in the channel
// server.broadcastMessage(realClient, channel->getName(), reply);

// // Remove the kicked user from the channel's user list
// channel->removeUser(userName);  // You might need to implement this in your channel class

// // Send the reply to the client who sent the KICK command (optional)
// server.sendToClient(client.getNickname(), reply);

// // Optionally, you could send a separate message to the kicked user, informing them that they have been kicked
// server.sendToClient(userName, "You have been kicked from " + channelName + " by " + client.getNickname());

	// channel->removeUser(userName);
    std::cout << "USER: " << userName << std::endl;
    server.removeChannel(client.getNickname());
    std::string reply;
    if (reason.empty())
        reply = ":" + client.getNickname() + " KICK " + channelName + " " + userName + "\r\n";
    else
        reply = ":" + client.getNickname() + " KICK " + channelName + " " + userName + " :" + reason + "\r\n";

    // server.sendToClient(userName, reply);// NEW
    server.broadcastMessage(realClient, channel->getName(), reply);
    server.sendToClient(client.getNickname(), reply);
    if (channel->getUsers().size() == 0)
        server.removeChannel(channelName);

    Client *newClient = server.getClient(userName);
    std::string reply2;
    channel->removeUser(userName);
    if (reason.empty())
        reply2 = ":" + userName + " PART " + channelName;
    else
        reply2 = ":" + userName + " PART " + channelName + " :" + reason;
    server.sendToClient(userName, reply2);
    server.broadcastMessage(newClient, channel->getName(), reply2);

}

void processInviteRequest(IClient& client, IServer& server, IChannel* channel, \
    const std::string& userName)
{
    channel->addInvitedUser(userName);
    std::string reply = ":" + client.getNickname() + " INVITE " +
        userName + " " + channel->getName();
    server.sendToClient(userName, reply);
    std::cout << reply << std::endl;
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
        server.broadcastMessage(realClient, client.getNickname(), reply);
        std::cout << reply << std::endl;
    }
    return;
}

void processSetTopicRequest(IClient& client, IServer& server, IChannel* channel, std::string newTopic)
{
    Client* realClient = dynamic_cast<Client*>(&client);

    std::string reply;
    channel->setTopic(newTopic.erase(0, 1));
    if (channel->getTopic().empty())
        reply = ":" + IRCname + " " + IRCreply::RPL_NOTOPIC + " " +
        client.getNickname() + " " + channel->getName() + " :No topic is set for this channel";
    else
        reply = ":" + IRCname + " " + IRCreply::RPL_TOPIC + " " +
                    client.getNickname() + " " + channel->getName() + " :" + channel->getTopic();
    server.sendToClient(client.getNickname(), reply);
    server.broadcastMessage(realClient, client.getNickname(), reply);
    std::cout << reply << std::endl;
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
        channel->setOnlyOperatorCanChangeTopic(true);
    else if (mode == "-t")
        channel->setOnlyOperatorCanChangeTopic(false);
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
void processModeThreeArgsRequest(IClient& client, IServer& server, IChannel* channel, std::string mode, std::string modeParamIdx2)
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
        std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_UNKNOWNMODE + " " +
            client.getNickname() + " " + mode + " :Unknown channel mode";
        server.sendToClient(client.getNickname(), errorMsg);
        std::cerr << errorMsg << std::endl;
    }
}
