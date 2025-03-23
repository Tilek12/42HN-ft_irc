/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:37:19 by ryusupov          #+#    #+#             */
/*   Updated: 2025/03/23 19:12:04 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command_handler.hpp"

void CommandHandler::Handle_command(Client *client, const std::string &command){

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
		std::string username, temp, realname;
		ss >> username >> temp >> temp;

		if (ss.peek() == ':') {
			ss.ignore();
			std::getline(ss, realname);
		}

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

		if (ss.peek() == ':'){
			ss.ignore();
			std::getline(ss, msg);
		}

		if (recipent.empty() || msg.empty()) {
			SendError(client, "No recipent provided or the message is empty!");
			return ;
		}

		Client *reciever = findClient(recipent);
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
	for (auto& n : clients) {
		if (n.second->getNickname() == nickname){
			return (true);
		}else {
			return (false);
		}
	}
}


Client *CommandHandler::findClient(std::string &nickname) {
	for (auto &nick : clients) {
		if (nick.second->getNickname() == nickname) {
			return nick.second;
		}else {
			return (nullptr);
		}
	}
}
