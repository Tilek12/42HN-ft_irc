/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command_handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:37:19 by ryusupov          #+#    #+#             */
/*   Updated: 2025/03/25 17:20:57 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CommandHandler.hpp"

void CommandHandler::handleCommand(Client *clinet, const std::string &command){

	std::stringstream ss(command);
	std::string parsed_command;
	ss >> parsed_command;

	if (parsed_command == "NICK") {
		std::string nickname;
		ss >> nickname;

		if (nickname.empty()) {
			SendError(clinet, "Nickname is not provided! Please enter a nickname!");
			return;
		}

		if (NickNameTaken(nickname)) {
			SendError (clinet, "The chosen nickname has already taken! Please choose another nickname!");
			return ;
		}

		clinet->setNickname(nickname);

		SendMessage(clinet, "Your nickname is now " + nickname);
	} else {
		SendError (clinet, "Unknown command!");
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
	for (auto& n : clients) {
		if (n.second->getNickname() == nickname){
			return (true);
		}else {
			return (false);
		}
	}
	return (true);
}
