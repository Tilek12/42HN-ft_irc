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
     