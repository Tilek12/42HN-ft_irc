/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:30:49 by ryusupov          #+#    #+#             */
/*   Updated: 2025/04/01 18:45:39 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client_c_h.hpp"

std::vector<std::string> CommandHandler::parseSpecialCommands(const std::string &command) {
	std::istringstream iss(command);
	std::string cmd;
	std::vector<std::string> arguments;
	iss >> cmd;

	static const std::unordered_set<std::string> setOfCmds = {
		"USER", "PRIVMSG", "NOTICE"
	};

	if (setOfCmds.find(cmd) != setOfCmds.end()) {
		/*Pushing command to the vector*/
		arguments.push_back(cmd);

		if (cmd == "PRIVMSG" || cmd == "NOTICE") {
			std::string target, message;
			if (!(iss >> target)){
				std::cerr << "Error: Not target provided for " << cmd << std::endl;
				return {};
			}
			arguments.push_back(target);

			std::getline(iss, message);
			if (!message.empty() && message[0] == ' ') message.erase(0, 1);
			if (message.empty() && message[0] != ':') {
				std::cerr << "Error: No message to send!" << std::endl;
				return {};
			}
			message.erase(0, 1);
			arguments.push_back(message);
		}
		else if (cmd == "USER") {
			std::string username, hostname, servername, realname;

			if (!(iss >> username >> hostname >> servername)) {
				std::cerr << "Error: Incomplete USER command!" << std::endl;
				return {};
			}
			arguments.push_back(username);
			arguments.push_back(hostname);
			arguments.push_back(servername);

			std::getline(iss, realname);
			if (!realname.empty() && realname[0] == ' ') realname.erase(0, 1);
			if (!realname.empty() && realname[0] == ':') realname.erase(0, 1); //<----- here im removing :
			arguments.push_back(realname);
		}
	}
	std::cout << "test cmd: " << arguments.size() << std::endl;
	return (arguments);
}


std::vector<std::string>CommandHandler::parseCommand(const std::string &command) {
	std::vector<std::string> arguments;
	std::istringstream iss(command);
	std::string arg1, arg2, arg3, arg4;

	/*Set of all the available commands*/
	static const std::unordered_set<std::string> setOfCmds = {
		"JOIN", "PART", "QUIT", "NICK", "USER", "PRIVMSG", "NOTICE",
        "PING", "PONG", "MODE", "TOPIC", "KICK", "INVITE"
	};

	/*Checking for command if it is existing command and adding it to a vector*/
	iss >> arg1;
	if (setOfCmds.find(arg1) != setOfCmds.end()) {
		if (arg1 == "USER" || arg1 == "PRIVMSG" || arg1 == "NOTICE" || arg1 == "KICK"){
			arguments = parseSpecialCommands(command);
			return arguments;
		}
		arguments.push_back(arg1);
	}else
		std::cout << "Unknown command!" << std::endl;

	/*checking for a arg2 if they're provided and adding them to a vector*/
	if (!(iss >> arg2)) {
		std::cerr << "No arg2 provided!" << std::endl;
		return {};
	}
	arguments.push_back(arg2);

	/*checking for a password and adding them to a vector*/
	if (!(iss >> arg3))
		return arguments;
	arguments.push_back(arg3);

	/*checking if there is no any additional arguments provided*/
	if (iss.rdbuf()->in_avail() > 0) {
		std::cerr << "Error: There are extra argument provided please check and type again!" << std::endl;
		return {};
	}

	return arguments;
}


// void testParseJoinCommand(const std::string& commandLine) {
//     std::vector<std::string> parsed = parseCommand(commandLine);

//     if (!parsed.empty()) {
//         std::cout << "Channels: " << parsed[0] << std::endl;
//         std::cout << "Passwords: " << parsed[1] << std::endl;
//         std::cout << "Passwords: " << parsed[2] << std::endl;
//         // std::cout << "Passwords: " << parsed[3] << std::endl;
//         // std::cout << "Passwords: " << parsed[4] << std::endl;
//     } else {
//         std::cout << "Invalid command!" << std::endl;
//     }
// }

// int main() {
//     testParseJoinCommand("JOIN #chan1,#chan2,#chan3 pass1,pass2");
//     // testParseJoinCommand("PRIVMSG rustam :Hello there!");
// 	// testParseJoinCommand("USER testuser 0 * :realname real");
//     return 0;
// }
