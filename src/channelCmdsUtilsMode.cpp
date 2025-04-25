#include "../include/ChannelCmds.hpp"
#include "../include/IRCreply.hpp"
#include "../include/client_c_h.hpp"

void sendModeTwoArgsMsg(IClient& client, IServer& server, \
    IChannel* channel, std::string mode)
{
    std::string reply = ":" + client.getNickname() + "!@" + \
        client.getHostname() + " MODE " + channel->getName() + " " + mode;
    server.broadcastMessage(dynamic_cast<Client*>(&client), \
        channel->getName(), reply);
    server.sendToClient(client.getNickname(), reply);

}

void sendModeThreeArgsMsg(IClient& client, IServer& server, \
    IChannel* channel, std::string mode, std::string modeParamIdx2)
    {
        std::string reply = ":" + client.getNickname() + " MODE " + \
        channel->getName() + " " + mode + " " + modeParamIdx2;
        server.broadcastMessage(dynamic_cast<Client*>(&client), \
        channel->getName(), reply);
        server.sendToClient(client.getNickname(), reply);
    }

void sendErrorMode(IClient& client, IServer& server, std::string mode)
{
    std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_UNKNOWNMODE + \
        " " + client.getNickname() + " " + mode + " :Unknown channel mode";
    server.sendToClient(client.getNickname(), errorMsg);
}

void sendNoChannelReply(IClient& client, IServer& server, \
    std::string channelName)
{
    std::string reply = ":" + IRCname + " " + IRCerror::ERR_NOCHANMODES + " " + \
        client.getNickname() + " " + channelName + " :Channel does not exist";
    server.sendToClient(client.getNickname(), reply);
}
void processModeOneArgsRequest(IClient& client, IServer& server, \
    IChannel* channel)
{
    std::string modes = "+";
    std::string params;

    if (channel->getIsInviteOnly())
        modes += "i";
    if (channel->getOnlyOperatorCanChangeTopic())
        modes += "t";
    if (channel->getUserLimit() > 0)
    {
        modes += "l";
        params += " " + std::to_string(channel->getUserLimit());
    }
    if (channel->getHasPassword())
    {
        modes += "k";
        params += " " + channel->getPassword();
    }
    std::string reply = ":" + IRCname + " " + IRCreply::RPL_CHANNELMODEIS + " " + \
        client.getNickname() + " " + channel->getName() + " " + modes + params;
    server.sendToClient(client.getNickname(), reply);
}
