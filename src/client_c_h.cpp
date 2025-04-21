/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_c_h.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:37:19 by ryusupov          #+#    #+#             */
/*   Updated: 2025/04/21 18:48:04 by ryusupov         ###   ########.fr       */
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
		if (!(handleNickname(client, command)))
			return ;
	} else if (command[0] == "USER") {
		if (command.size() < 5 || command[1].empty() || command[4].empty()) {
			std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS + \
    		" " + client->getNickname() + " " + command[0] + " :Not enough parameters\r\n";
			server.sendToClient(client->getNickname(), errorMsg);
			return ;
		}

		if (!server.checkClientAuthentication(client))
			return ;

		client->setUsername(command[1]);
		client->setRealname(command[4]);

		client->markUserReceived();

		if (client->getIsResgistered()) {
			registerClient(client);
		}
	} else if (command[0] == "PRIVMSG") {

		if (command[2].empty()) {
			std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NOTEXTTOSEND + \
    		" " + client->getNickname() + " " + command[0] + " :No text to send\r\n";
			server.sendToClient(client->getNickname(), errorMsg);
			return ;
		}
		findTargetPrivmsg(client, command);
	} else if (command[0] == "WHO") {
		if (command[1].empty()) {
			server.sendToClient(client->getNickname(), ":server.name 461 " + client->getNickname() + " WHO :Not enough parameters\r\n");
			return ;
		}
		handleWhoCmd(client, command);
	}
}

/***************************************************************************/
/***************************WHO COMMAND*********************************/
/***************************************************************************/
void CommandHandler::handleWhoCmd(Client *client, std::vector<std::string> &command) {
	Channel *channel = server.getChannel(command[1]);
	if (!channel) {
		server.sendToClient(client->getNickname(), ":server.name 315 " + client->getNickname() + " " + command[1] + " :End of /WHO list\r\n");
		return ;
	}
	for (const std::string &nickname : channel->getUsers()) {
		Client *user = server.getClient(nickname);
		if (user) {
			std::string reply = ":server.name 352 " + client->getNickname() + " " + command[1] + " " +
                            user->getUsername() + " " + user->getHostname() + " " + "localhost" + " " +
                            user->getNickname() + " H :" + "0 " + user->getRealname() + "\r\n";
			server.sendToClient(client->getNickname(), reply);
		}
	}
	server.sendToClient(client->getNickname(), ":server.name 315 " + client->getNickname() + " " + command[1] + " :End of /WHO list\r\n");
}


/***************************************************************************/
/***************************PRIVMSG COMMAND*********************************/
/***************************************************************************/
void	CommandHandler::findTargetPrivmsg(Client *client, std::vector<std::string> &command) {
	std::string target = command[1];
	std::string msg = command[2];

	std::vector<std::string> targets = splitTargets(target);

	if (targets.size() > 0) {
		for (size_t i = 0; i < targets.size(); i++) {
			if (targets[i][0] == '#'){
				std::string prefix = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname();
				std::string fullMessage = prefix + " PRIVMSG " + targets[i] + " :" + msg + "\r\n";
				server.broadcastMessage(client, targets[i], fullMessage);
			}else {
				if (!server.getClient(targets[i])){
					std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NOSUCHNICK + \
					" " + client->getNickname() + " " + targets[i] + " :No such user found\r\n";
					server.sendToClient(client->getNickname(), errorMsg);
					continue;
				}
				std::string prefix = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname();
				std::string fullMessage = prefix + " PRIVMSG " + targets[i] + " :" + msg + "\r\n";
				server.sendToClient(targets[i], fullMessage);
			}
		}
	}
}


std::vector<std::string> CommandHandler::splitTargets(std::string &target) {
	std::vector<std::string> result;

	if (target.find(',') != std::string::npos) {
		std::string temp;
		std::istringstream tokens(target);

		while (getline(tokens, temp, ','))
			result.push_back(temp);

		return result;
	}
	result.push_back(target);
	return result;
}

/***************************************************************************/
/****************************USER COMMAND***********************************/
/***************************************************************************/
void	CommandHandler::registerClient(Client *client) {
	client->setIsRegistered(true);

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
		std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NICKNAMEINUSE + \
    " " + client->getNickname() + " " + newNick + " :Nickname is already in use\r\n";
		server.sendToClient(client->getNickname(), errorMsg);
		return false;
	}

	client->setNickname(newNick);

	if (client->getIsResgistered()) {
		std::string nickChangeMsg = ":" + oldNick + "!" + client->getUsername() + "@" + client->getHostname() + " NICK :" + newNick;
		updateNicknameInChannels(client, oldNick, newNick, nickChangeMsg);
		server.broadcastMessage(client, "", nickChangeMsg);
		server.sendToClient(client->getNickname(), nickChangeMsg);
	} else {
		// std::string nickChangeMsg = ":" + oldNick + "!" + client->getUsername() + "@" + client->getHostname() + " NICK :" + newNick;
		// server.sendToClient(client->getNickname(), nickChangeMsg);
		// client->setIsRegistered(true);
		server.sendToClient(newNick, ":irc.server.com 001 " + newNick + " :Welcome to the IRC network, " + newNick);
 		client->setIsRegistered(true);
 		server.addClient(client);
	}
	return true;
}

void CommandHandler::updateNicknameInChannels(Client *client, const std::string &oldNick, const std::string &newNick, std::string msg) {
	for (auto &&channelPair : server.getChannels()) {
		Channel *channel = channelPair.second;
		std::vector<std::string> &users = channel->getUsers();

		for (std::string &user : users) {
			if (user == oldNick) {
				user = newNick;
				server.broadcastMessage(client, channelPair.first, msg);
				break;
			}
		}
	}
}

// void CommandHandler::SendMessage(const std::string &msg){
// 	std::cout << "New message: " << msg << std::endl;
// }

// void CommandHandler::SendError(const std::string &msg) {
// 	std::string message = "Error" + msg;
// 	SendMessage(message);
// }

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
		"CAP", "PING", "NOTICE", "PASS", "QUIT"
	};

	static std::unordered_set<std::string> channelCmds = {
		"JOIN", "PART", "KICK", "MODE", "TOPIC", "INVITE"
	};

	static std::unordered_set<std::string> clientCmds = {
		"PRIVMSG", "NICK", "USER", "WHO"
	};

	if (serverCmds.find(args[0]) != serverCmds.end()){
		serverCmdHandler(&server, client);
	}
	else if (channelCmds.find(args[0]) != channelCmds.end()) {
		channelCmdHandler(*client, server, args);
	}
	else if (clientCmds.find(args[0]) != clientCmds.end()) {
		clientCmdHandler(client, args);
	}
}
