/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_c_h.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llacsivy <llacsivy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:37:19 by ryusupov          #+#    #+#             */
/*   Updated: 2025/04/08 18:20:05 by llacsivy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "../include/client_c_h.hpp"
#include "../include/Client.hpp"
#include "../include/ChannelCmds.hpp"
#include "../include/IClient.hpp"

CommandHandler::CommandHandler(Server& srv) : server(srv) {
	//
}

void CommandHandler::clientCmdHandler(Client *client, std::vector<std::string> &command){

	if (command[0] == "NICK") {

		if (command[1].empty()) {
			server.sendToClient(client->getNickname(), "Nickname is not provided! Please enter a nickname!");
			return;
		}

		if (NickNameTaken(command[1])) {
			server.sendToClient(client->getNickname(), "The chosen nickname has already taken! Please choose another nickname!");
			return ;
		}

		client->setNickname(command[1]);
		server.sendToClient(client->getNickname(), ":irc.server.com 001 " + command[1] + " :Welcome to the IRC network, " + command[1]);
		server.addClient(client);
	} else if (command[0] == "USER") {
		if (command[1].empty() || command[4].empty()) {
			SendError(client, "The username or realname is not provided!");
			return ;
		}

		client->setUsername(command[1]);
		client->setRealname(command[2]);

				// 	// 001 RPL_WELCOME
		server.sendToClient( client->getSocketFd(), ":" + IRCname + " 001 " + client->getNickname() +
							" :Welcome to the IRC Network " + client->getNickname() + "!" + command[1] + "@" + client->getHostname() );
			// 002 RPL_YOURHOST
		server.sendToClient( client->getSocketFd(), ":" + IRCname + " 002 " + client->getNickname() +
							" :Your host is " + IRCname + ", running version 1.0" );
			// 003 RPL_CREATED
		server.sendToClient( client->getSocketFd(), ":" + IRCname + " 003 " + client->getNickname() +
							" :This server was created " + server.getCreationTime() );
			// 004 RPL_MYINFO
		server.sendToClient( client->getSocketFd(), ":" + IRCname + " 004 " + client->getNickname() +
							" " + IRCname + " 1.0 +aiow +ntklovb" );
		SendMessage(client, "Welcome! " + command[1] + "!");
	} else if (command[0] == "PRIVMSG") {

		if (command[1].empty()) {
			SendError(client, "No recipent provided!");
			return ;
		}
		if (command[2].empty()) {
			SendError(client, "Empty msg provided!");
			return ;
		}
		server.sendToClient(command[1], command[2]);
	}
}

void CommandHandler::SendMessage(IClient *client, const std::string &msg){
	std::cout << "New message: " << msg << std::endl;
}

void CommandHandler::SendError(Client *client, const std::string &msg) {
	std::string message = "Error" + msg;
	SendMessage(client, message);
}

bool CommandHandler::NickNameTaken(std::string &nickname) {

	if (server.getClient(nickname))
		return true;
	return (false);
}

void	CommandHandler::MainCommandHandller(Client *client, std::vector<std::string> &args){
	static std::unordered_set<std::string> serverCmds = {
		"CAP", "PING", "NOTICE"
	};

	static std::unordered_set<std::string> channelCmds = {
		"JOIN", "PART", "KICK", "MODE", "TOPIC", "INVITE"
	};

	static std::unordered_set<std::string> clientCmds = {
		"PRIVMSG", "NICK", "USER"
	};

	if (serverCmds.find(args[0]) != serverCmds.end())
	{
		std::cout << args[0] << " = " << args[1] << std::endl;
		serverCmdHandler(&server, client);
	}
	else if (channelCmds.find(args[0]) != channelCmds.end()) {
		channelCmdHandler(*client, server, args);
	}
	else if (clientCmds.find(args[0]) != clientCmds.end()) {
		clientCmdHandler(client, args);
	}
}
