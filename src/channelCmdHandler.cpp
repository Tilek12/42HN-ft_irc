/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelCmdHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llacsivy <llacsivy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:45:42 by llacsivy          #+#    #+#             */
/*   Updated: 2025/04/26 17:45:48 by llacsivy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "../include/Server.hpp"
#include "../include/ChannelCmds.hpp"
#include "../include/IRCerror.hpp"

void channelCmdHandler(IClient& client, IServer& server, \
	std::vector<std::string>& args)
{
	std::string msg;
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
		msg = ":" + IRCname + " " + IRCerror::ERR_UNKNOWNCOMMAND + \
		" " + client.getNickname() + " " +  args[0] + " :Unknown command";
		server.sendToClient(client.getNickname(), msg);
}
