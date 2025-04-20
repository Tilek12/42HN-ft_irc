#include "../include/ChannelCmds.hpp"
#include "../include/IRCreply.hpp"
#include "../include/client_c_h.hpp"

void handleNewChannel(IServer& server, IChannel*& channel,
	const std::string& channelName,
	const std::string& password, IClient& client)
{
	channel = server.createChannel(channelName);
	channel->addOperator(client.getNickname());
	channel->setHasPassword(true);
	channel->setPassword(password);
}

void sendNameReplies(Client* client, IChannel* channel, IServer& server)
{
	std::string userList;
    for (auto &&i : channel->getUsers()) {
        if (channel->isOperator(i))
            userList += "@" + i + " ";
        else
            userList += i + " ";
    }
    std::string reply353 = ":" + IRCname + " " + IRCreply::RPL_NAMREPLY + " " +
                           client->getNickname() + " = " + channel->getName() + 
						   " :" + userList;
    server.sendToClient(client->getNickname(), reply353);
    std::string reply366 = ":" + IRCname + " " + IRCreply::RPL_ENDOFNAMES + " " +
                           client->getNickname() + " " + channel->getName() + 
						   " :End of /NAMES list";
    server.sendToClient(client->getNickname(), reply366);
}