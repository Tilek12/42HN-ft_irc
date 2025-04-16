/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:30:49 by ryusupov          #+#    #+#             */
/*   Updated: 2025/04/16 12:37:27 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client_c_h.hpp"

//TODO: Need to parse KICK commands too
//FIXME: Needs attention
std::vector<std::string>CommandHandler::parseSpecialCommands(const std::string &command, const std::unordered_set<std::string> &setOfCmds) {
	std::istringstream iss(command);
	std::string cmd;
	std::vector<std::string> arguments;
	iss >> cmd;

	if (setOfCmds.find(cmd) != setOfCmds.end()) {
		/*Pushing command to the vector*/
		arguments.push_back(cmd);
		/*Check for commands such as PRIVMSG and NOTICE*/
		if ((cmd == "PRIVMSG") && !handlePrivMsgNotice(iss, arguments))
			return {};
		else if (cmd == "USER" && !handleUser(iss, arguments))
			return {};
		else if (cmd == "TOPIC" && !handleTopic(iss, arguments))
			return {};
		else if (cmd == "MODE" && !handleMode(iss, arguments))
			return {};
		else if (cmd == "KICK" && !handleKick(iss, arguments))
			return {};
		else if (cmd == "PING" && !handlePing(iss, arguments))
			return {};
		else if (cmd == "CAP" && !handleCap(iss, arguments))
			return {};
		else if (cmd == "NOTICE" && !handleNotice(iss, arguments))
			return {};
		else if (cmd == "PART" && !handlePart(iss, arguments))
			return {};
		}
	return (arguments);
}

std::vector<std::string>CommandHandler::parseCommand(const std::string &command) {
	std::vector<std::string> arguments;
	std::istringstream iss(command);
	std::string arg1;

	/*Set of all the available commands*/
	static const std::unordered_set<std::string> setOfAllCmds = {
		"JOIN", "QUIT", "NICK", "INVITE", "WHO"
	};
	/*Set of special commands with more that three arguments required*/
	static const std::unordered_set<std::string> setOfSpecialCmds = {
		"USER", "PRIVMSG", "NOTICE", "KICK", "TOPIC", "MODE", "PING", "CAP", "PART"
	};
	/*Checking for command if it is existing command and adding it to a vector*/
	iss >> arg1;
	if (setOfAllCmds.find(arg1) != setOfAllCmds.end())
		arguments.push_back(arg1);
	else if (setOfSpecialCmds.find(arg1) != setOfSpecialCmds.end()){
		arguments.push_back(arg1);
		arguments = parseSpecialCommands(command, setOfSpecialCmds);
		return arguments;
	}
	else
		return {};
	if (!handleSimpleCommands(iss, arguments))
		return {};

	return arguments;
}

//TODO::PART COMMAND

// void testParseJoinCommand(const std::string& commandLine) {
//     std::vector<std::string> parsed = parseCommand(commandLine);

//     if (!parsed.empty()) {
//         std::cout << "Command: " << parsed[0] << std::endl;
//         std::cout << "Argument[1]: " << parsed[1] << std::endl;
//         std::cout << "Argument[2]: " << parsed[2] << std::endl;
//         // std::cout << "Argument[3]: " << parsed[3] << std::endl;
//         // std::cout << "Passwords: " << parsed[4] << std::endl;
//     } else {
//         std::cout << "Invalid command!" << std::endl;
//     }
// }

// int main() {
//     // testParseJoinCommand("JOIN #chan1,#chan2,#chan3 pass1,pass2");
//     testParseJoinCommand("asdf rustam :Hello");
// 	// testParseJoinCommand("USER testuser 0 * :realname real");
// 		// TOPIC #channel1 :block balbal
// 		// MODE #channel +o/-o USER {o, k, i, l, t}
// 		//PING iutuyt jjyty juuyt
// 		// PONG :hfghf jghjg hjghjg
//     return 0;
// }
