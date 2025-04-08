#include <set>

#include "../include/Server.hpp"
#include "../include/ChannelCmds.hpp"

void channelCmdHandler(IClient& client, IServer& server, \
	std::vector<std::string>& args)
{
	if (args[0] == "JOIN")
	{
		args.erase(args.begin());
		ChannelCmds::joinChannelCmd(client, server, args);
	}
	else if (args[0] == "PART")
	{
		args.erase(args.begin());
		ChannelCmds::partChannelCmd(client, server, args);
	}
	else if (args[0] == "KICK")
	{
		args.erase(args.begin());
		ChannelCmds::kickUserCmd(client, server, args);
	}
	else if (args[0] == "INVITE")
	{
		args.erase(args.begin());
		ChannelCmds::inviteUserCmd(client, server, args);
	}
	else if (args[0] == "TOPIC")
	{
		args.erase(args.begin());
		ChannelCmds::topicUserCmd(client, server, args);
	}
	else if (args[0] == "MODE")
	{
		args.erase(args.begin());
		ChannelCmds::modeChannelCmd(client, server, args);
	}
	else
		std::cerr << "Error: command " << args[0] << "not found!" << std::endl;
	
}
