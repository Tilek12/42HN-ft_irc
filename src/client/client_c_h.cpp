/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_c_h.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:37:19 by ryusupov          #+#    #+#             */
/*   Updated: 2025/03/24 19:56:04 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_c_h.hpp"
#include "client.hpp"

CommandHandler::CommandHandler(Server& srv) : server(srv) {
	//
}

void CommandHandler::Handle_client_command(Client *client, const std::string &command){

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

		Client *reciever = server.findClient(recipent);
		std::cout << reciever << std::endl;
		if (!reciever) {
			SendError(client, "Reciever with the " + recipent + " nickname was not found!");
			return ;
		}

		SendMessage(reciever, "Message from: " + client->getNickname() + ": " + msg);
	} else {
		SendError (client, "Unknown command!");
	}
}

void CommandHandler::SendMessage(Client *client, const std::string &msg){
	std::cout << "New message: " << msg << std::endl;
}

void CommandHandler::SendError(Client *client, const std::string &msg) {
	std::string message = "Error" + msg;
	SendMessage(client, message);
}

bool CommandHandler::NickNameTaken(std::string &nickname) {
	for (auto& n : server.clients) {
		if (n.second->getNickname() == nickname){
			return (true);
		}
	}
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
