/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:18:33 by ryusupov          #+#    #+#             */
/*   Updated: 2025/04/16 17:07:35 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client_c_h.hpp"

/*------Parsing PART command------*/
bool CommandHandler::handlePart(std::istringstream &iss, std::vector<std::string> &args) {
	std::string channel, reason;

	if (!(iss >> channel))
		return false;
	args.push_back(channel);

	std::getline(iss, reason);
	if (!reason.empty() && reason[0] == ' ')
		reason.erase(0, 1);
	if (!(reason.empty()))
		args.push_back(reason);
	return true;
}

/*------Parsing PRIVMSG and NOTICE commands------*/
bool CommandHandler::handlePrivMsgNotice(std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string target, message;
		if (!(iss >> target)){
			std::cerr << "Error: Not target provided for " << std::endl;
			return false;
		}
		arguments.push_back(target);

		std::getline(iss, message);
		if (!message.empty() && message[0] == ' ') message.erase(0, 1);
		if (message.empty() || message[0] != ':')
			return {};
		message.erase(0, 1);
		arguments.push_back(message);

	return true;
}

/*------Parsing NOTICE command*/
bool CommandHandler::handleNotice(std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string lagcheck, msg, lagData;
	bool found = false;

	while (iss >> lagcheck) {
		if (lagcheck == ":\001LAGCHECK") {
			found = true;
			//the case if there is no witespace between LAGCHECK and the rest of the msg
			if (lagcheck.size() > 10) {
				arguments.push_back(msg);
				arguments.push_back(":LAGCHECK");
				arguments.push_back(lagcheck.substr(11));
				return true;
			}

			arguments.push_back(msg);
			arguments.push_back(":LAGCHECK");

			std::getline(iss, lagData);
			if (!lagData.empty() && lagData[0] == ' ')
			lagData.erase(0, 1);
			if (!(lagData.empty()))
			arguments.push_back(lagData);
			return true;
		}
		if (!msg.empty()) msg += " ";
		msg += lagcheck;
	}
	return false;
}

/*------Parsing USER command------*/
bool CommandHandler::handleUser(std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string username, hostname, servername, realname;

		if (!(iss >> username >> hostname >> servername)) {
			std::cerr << "Error: Incomplete USER command!" << std::endl;
			return false;
		}

		arguments.push_back(username);
		arguments.push_back(hostname);
		arguments.push_back(servername);

		std::getline(iss, realname);
		if (!realname.empty() && realname[0] == ' ') realname.erase(0, 1);

		if (realname.empty() || realname[0] != ':')
			return false;
		realname.erase(0, 1);
		arguments.push_back(realname);

	return true;
}

/*------Parsing TOPIC command------*/
bool CommandHandler::handleTopic(std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string channel, topic;

		if (!(iss >> channel)) {
			std::cerr << "Error: No channel provided for TOPIC command!" << std::endl;
			return false;
		}
		arguments.push_back(channel);

		std::getline(iss, topic);
		if (!(topic.empty() && topic[0] == ' ')) topic.erase(0, 1);

		if (topic.empty() || topic[0] != ':')
			return false;
		topic.erase(0, 1);

		if (!topic.empty())
			arguments.push_back(topic);

	return true;
}

/*------Parsing MODE command------*/
bool CommandHandler::handleMode(std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string channel, flag, user;
		if (!(iss >> channel)) {
			std::cout << "Error: No channel provided for MODE command!" << std::endl;
			return false;
		}
		arguments.push_back(channel);

		if (iss >> flag) {
			if (flag.size() == 1 && flag[0] == 'b') {
				arguments.push_back(flag);
			} else if (flag.size() == 2 && (flag[0] == '+' || flag[0] == '-'))
				arguments.push_back(flag);
		}
		std::getline(iss, user);
		if (!(user.empty() && user[0] == ' ')) user.erase(0, 1);

		if (!(user.empty()))
			arguments.push_back(user);
	return true;
}

/*------Parsing Simple commands------*/
bool CommandHandler::handleSimpleCommands(std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string arg2, arg3;

	if (!(iss >> arg2)) {
		std::cerr << "No arg2 provided!" << std::endl;
		return {};
	}
	arguments.push_back(arg2);

	/*checking for a password and adding them to a vector*/
	if (!(iss >> arg3))
		return true;
	arguments.push_back(arg3);

	/*checking if there is no any additional arguments provided*/
	if (iss.rdbuf()->in_avail() > 0) {
		std::cerr << "Error: There are extra argument provided please check and type again!" << std::endl;
		return false;
	}

	return true;
}

/*------Parsing KICK command------*/
bool CommandHandler::handleKick(std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string channel, user, reason;

	if (!(iss >> channel) || std::string("#!&+").find(channel[0]) == std::string::npos)
		return false;
	arguments.push_back(channel);

	if (!(iss >> user))
		return false;
	arguments.push_back(user);

	if (iss.rdbuf()->in_avail() > 0){
		std::getline(iss, reason);
		if (!(reason.empty()) && reason[0] == ' ') reason.erase(0, 1);
		if (reason.empty() || reason[0] != ':')
			return false;
		arguments.push_back(reason);
	}
	return true;
}


/*------Parsing PING command------*/
bool CommandHandler::handlePing(std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string arg2;

	if (iss.rdbuf()->in_avail() > 0) {
		std::getline(iss, arg2);
		if (!(arg2.empty()) && arg2[0] == ' ') arg2.erase(0, 1);
		if (arg2.empty())
			return false;
		arguments.push_back(arg2);
	}
	return true;
}

bool CommandHandler::handleCap(std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string arg1, arg2;

	if (!(iss >> arg1)){
		std::cerr << "No arg2 provided!" << std::endl;
		return {};
	}
	arguments.push_back(arg1);
	if (iss.rdbuf()->in_avail() > 0) {
		std::getline(iss, arg2);
		if (!(arg2.empty()) && arg2[0] == ' ') arg2.erase(0, 1);
		if (arg2.empty())
			return false;
		arguments.push_back(arg2);
	}
	return true;
}
