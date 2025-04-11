/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_c_h.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:37:19 by ryusupov          #+#    #+#             */
/*   Updated: 2025/04/11 20:34:55 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "../include/client_c_h.hpp"
#include "../include/Client.hpp"
#include "../include/ChannelCmds.hpp"
#include "../include/IClient.hpp"

//FIXME: PRIVMSG IS SENDING THE TEXT TWICE IN CHANNELS
CommandHandler::CommandHandler(Server& srv) : server(srv) {
	//
}

void CommandHandler::clientCmdHandler(Client *client, std::vector<std::string> &command){

		if (command[0] == "NICK"){
			if (!(handleNickname(client, command)))
				return ;

		}else if (command[0] == "USER") {
		if (command.size() < 5 || command[1].empty() || command[4].empty()) {
			SendError(client, "The username or realname is not provided!");
			return ;
		}

		client->setUsername(command[1]);
		client->setRealname(command[2]);

		client->markUserReceived();

		if (client->getIsResgistered()) {
			registerClient(client);
		}
	} else if (command[0] == "PRIVMSG") {

		if (command[1].empty()) {
			server.sendToClient(client->getNickname(), "No recipent provided!");
			return ;
		}
		if (command[2].empty()) {
			server.sendToClient(client->getNickname(), "Empty msg provided!");
			return ;
		}
		findTargetPrivmsg(client, command);
	}
}

/***************************************************************************/
/***************************PRIVMSG COMMAND*********************************/
/***************************************************************************/
void	CommandHandler::findTargetPrivmsg(Client *client, std::vector<std::string> &command) {
	std::string target = command[1];
	std::string msg = command[2];

	if (target[0] == '#') {
		std::string prefix = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname();
        std::string fullMessage = prefix + " PRIVMSG " + target + " :" + msg + "\r\n";
		server.broadcastMessage(client, target, fullMessage);
	}
	else {
		std::string prefix = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname();
        std::string fullMessage = prefix + " PRIVMSG " + target + " :" + msg + "\r\n";
		server.sendToClient(target, fullMessage);
	}
}



/***************************************************************************/
/****************************USER COMMAND***********************************/
/***************************************************************************/
void	CommandHandler::registerClient(Client *client) {
	client->setIsRegistered(true);
	server.addClient(client);

	std::string nick = client->getNickname();
	std::string user = client->getUsername();
	std::string host = client->getHostname();

	server.sendToClient(client->getSocketFd(), ":" + IRCname + " 001 " + nick + " :Welcome to the IRC Network " + nick + "!" + user + "@" + host);
	server.sendToClient(client->getSocketFd(), ":" + IRCname + " 002 " + nick + " :Your host is " + IRCname + ", running version 1.0");
	server.sendToClient(client->getSocketFd(), ":" + IRCname + " 004 " + nick + " " + IRCname + " 1.0 +aiow +ntklovb");

}

/***************************************************************************/
/****************************NICK COMMAND***********************************/
/***************************************************************************/
bool CommandHandler::handleNickname(Client *client, std::vector<std::string> &command) {

	if (command.size() < 2 || command[1].empty()) {
		server.sendToClient(client->getNickname(), "431 * :Nickname is not provided. Please enter a Nickname!");
		return false;
	}

	std::string newNick = command[1];
	std::string oldNick = client->getNickname();

	if (NickNameTaken(newNick)) {
		server.sendToClient(client->getNickname(), "433 * " + newNick + " :The chosen nickname has already taken!");
		return false;
	}

	client->setNickname(newNick);

	if (client->getIsResgistered()) {
		std::string nickChangeMsg = ":" + oldNick + "!" + client->getUsername() + "@" + client->getHostname() + " NICK :" + newNick;
		server.sendToClient(client->getNickname(), nickChangeMsg);
	} else {
		server.sendToClient(newNick, ":irc.server.com 001 " + newNick + " :Welcome to the IRC network, " + newNick);

		client->setIsRegistered(true);
		server.addClient(client);
	}
	return true;
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

/***************************************************************************/
/***********************COMMAND HANDLER METHOD******************************/
/***************************************************************************/
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
