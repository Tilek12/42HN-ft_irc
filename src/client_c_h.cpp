/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_c_h.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llacsivy <llacsivy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:37:19 by ryusupov          #+#    #+#             */
/*   Updated: 2025/03/31 14:55:29 by llacsivy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client_c_h.hpp"
#include "../include/Client.hpp"
#include "../include/ChannelCmds.hpp"

CommandHandler::CommandHandler(Server& srv) : server(srv) {
	//
}

void CommandHandler::handleCommand(Client *client, const std::string &command){

	std::stringstream ss(command);
	std::string parsed_command;
	ss >> parsed_command;

	if (parsed_command == "NICK") {
		std::string nickname;
		ss >> nickname;

		if (nickname.empty()) {
			SendError(client, "Nickname is not provided! Please enter a nickname!");
			return;
		}

		if (NickNameTaken(nickname)) {
			SendError (client, "The chosen nickname has already taken! Please choose another nickname!");
			return ;
		}

		client->setNickname(nickname);
		server.addClient(client);
		SendMessage(client, "Your nickname is now " + nickname);
	} else if (parsed_command == "USER") {
		std::string username, mode, unused, realname;
		ss >> username >> mode >> unused;

		if (ss.peek() == ':') {
			ss.ignore();
		}
		std::getline(ss, realname);

		if (username.empty() || realname.empty()) {
			SendError(client, "The username or realname is not provided!");
			return ;
		}

		client->setUsername(username);
		client->setRealname(realname);

		SendMessage(client, "Welcome! " + username + "!");
	} else if (parsed_command == "PRIVMSG") {
		std::string recipent, msg;
		ss >> recipent;

		if (recipent.empty()) {
			SendError(client, "No recipent provided!");
			return ;
		}

		if (ss.peek() == ':'){
			ss.ignore();
		}
		std::getline(ss, msg);

		if (msg.empty()) {
			SendError(client, "Empty msg provided!");
			return ;
		}

		Client *reciever = server.getClient(recipent);
		std::cout << reciever << std::endl;
		if (!reciever) {
			SendError(client, "Reciever with the " + recipent + " nickname was not found!");
			return ;
		}
		server.addClient(client);
		SendMessage(reciever, "Message from: " + std::to_string(client->getSocketFd()) + " " + client->getNickname() + ": " + msg);
	} else if (parsed_command == "JOIN") {
		std::vector<std::string> joinParams;
		joinParams.push_back("#channel");
		ChannelCmds::joinChannelCmd(*client, server, joinParams);
	} else if (parsed_command == "PART") {
		std::vector<std::string> partParams;
		partParams.push_back("#channel");
		partParams.push_back("because not interested anymore");
		ChannelCmds::partChannelCmd(*client, server, partParams);
	} else if (parsed_command == "KICK") {
		std::vector<std::string> kickParams;
		kickParams.push_back("#channel");
		kickParams.push_back("user1");
		kickParams.push_back("unpolite user");
		ChannelCmds::kickUserCmd(*client, server, kickParams);
	} else {
		SendError (client, "Unknown command!");
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

// Client *CommandHandler::findClient(std::string &nickname) {
// 	for (auto &nick : clients) {
// 		if (nick.second->getNickname() == nickname) {
// 			return nick.second;
// 		}
// 	}
// 	return (nullptr);
// }
