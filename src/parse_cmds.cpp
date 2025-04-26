/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:30:49 by ryusupov          #+#    #+#             */
/*   Updated: 2025/04/26 14:31:00 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client_c_h.hpp"

std::vector<std::string>CommandHandler::parseSpecialCommands(Client *client, const std::string &command, const std::unordered_set<std::string> &setOfCmds) {
	std::istringstream iss(command);
	std::string cmd;
	std::vector<std::string> arguments;
	iss >> cmd;

	if (setOfCmds.find(cmd) != setOfCmds.end()) {
		/*Pushing command to the vector*/
		arguments.push_back(cmd);
		/*Check for commands such as PRIVMSG and NOTICE*/
		if ((cmd == "PRIVMSG") && !handlePrivMsgNotice(client, iss, arguments))
			return {};
		else if (cmd == "USER" && !handleUser(client, iss, arguments))
			return {};
		else if (cmd == "TOPIC" && !handleTopic(client, iss, arguments))
			return {};
		else if (cmd == "MODE" && !handleMode(client, iss, arguments))
			return {};
		else if (cmd == "KICK" && !handleKick(iss, arguments))
			return {};
		else if (cmd == "PING" && !handlePing(iss, arguments))
			return {};
		else if (cmd == "CAP" && !handleCap(iss, arguments))
			return {};
		else if (cmd == "NOTICE" && !handleNotice(iss, arguments))
			return {};
		else if (cmd == "PART" && !handlePart(client, iss, arguments))
			return {};
		else if (cmd == "QUIT" && !handleQuit(client, iss, arguments))
			return {};
		}
	return (arguments);
}

std::vector<std::string>CommandHandler::parseCommand(Client *client, const std::string &command) {
	std::vector<std::string> arguments;
	std::istringstream iss(command);
	std::string arg1;

	/*Set of all the available commands*/
	static const std::unordered_set<std::string> setOfAllCmds = {
		"JOIN", "NICK", "INVITE", "WHO", "PASS"
	};
	/*Set of special commands with more that three arguments required*/
	static const std::unordered_set<std::string> setOfSpecialCmds = {
		"USER", "PRIVMSG", "NOTICE", "KICK", "TOPIC", "MODE", "PING", "CAP", "PART", "QUIT"
	};
	/*Checking for command if it is existing command and adding it to a vector*/
	iss >> arg1;
	if (setOfAllCmds.find(arg1) != setOfAllCmds.end()) {
		arguments.push_back(arg1);
	}
	else if (setOfSpecialCmds.find(arg1) != setOfSpecialCmds.end()){
		arguments.push_back(arg1);
		arguments = parseSpecialCommands(client, command, setOfSpecialCmds);
		return arguments;
	}
	else{
		arguments.push_back(arg1);
		return arguments;
	}
	if (!this->handleSimpleCommands(client, iss, arguments))
		return {};
	return arguments;
}
