/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 19:18:33 by ryusupov          #+#    #+#             */
/*   Updated: 2025/04/26 14:55:31 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/client_c_h.hpp"

bool CommandHandler::handleQuit(Client *client, std::istringstream &iss, std::vector<std::string> &args) {
	std::string reason;

	(void)client;
	std::getline(iss, reason);
	if (!reason.empty() && reason[0] == ' ')
		reason.erase(0, 1);
	if (!(reason.empty()))
		args.push_back(reason);
	return true;
}

/*------Parsing PART command------*/
bool CommandHandler::handlePart(Client *client, std::istringstream &iss, std::vector<std::string> &args) {
	std::string channel, reason;

	if (!(iss >> channel)) {
		std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS + \
    		" " + client->getNickname() + " " + args[0] + " :No recipient given\r\n";
			server.sendToClient(client->getNickname(), errorMsg);
			return false;
	}

	channel = lowerCaseChannelName(channel);
	args.push_back(channel);

	std::getline(iss, reason);
	if (!reason.empty() && reason[0] == ' ')
		reason.erase(0, 1);
	if (!(reason.empty()))
		args.push_back(reason);
	return true;
}

/*------Parsing PRIVMSG and NOTICE commands------*/
bool CommandHandler::handlePrivMsgNotice(Client *client, std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string target, message;
		if (!(iss >> target)){
			std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS + \
    		" " + client->getNickname() + " " + arguments[0] + " :No recipient given\r\n";
			server.sendToClient(client->getNickname(), errorMsg);
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
	// bool found = false;

	while (iss >> lagcheck) {
		if (lagcheck == ":\001LAGCHECK") {
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
bool CommandHandler::handleUser(Client *client, std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string username, hostname, servername, realname;

		if (!(iss >> username >> hostname >> servername)) {
			std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS + \
    		" " + client->getNickname() + " " + arguments[0] + " :Not enough parameters\r\n";
			server.sendToClient(client->getNickname(), errorMsg);
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
bool CommandHandler::handleTopic(Client *client, std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string channel, topic;

		if (!(iss >> channel)) {
			std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS + \
    		" " + client->getNickname() + " " + arguments[0] + " :Not enough parameters\r\n";
			server.sendToClient(client->getNickname(), errorMsg);
			return false;
		}

		channel = lowerCaseChannelName(channel);
		arguments.push_back(channel);

		std::getline(iss, topic);
		if (!(topic.empty() && topic[0] == ' ')) topic.erase(0, 1);

		if (topic.empty() || topic[0] != ':')
			return true;

		if (!topic.empty())
			arguments.push_back(topic);

	return true;
}

/*------Parsing MODE command------*/
bool CommandHandler::handleMode(Client *client, std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string channel, flag, user;
		if (!(iss >> channel)) {
			std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS + \
    		" " + client->getNickname() + " " + arguments[0] + " :Not enough parameters\r\n";
			server.sendToClient(client->getNickname(), errorMsg);
			return false;
		}
		channel = lowerCaseChannelName(channel);
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
bool CommandHandler::handleSimpleCommands(Client *client, std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string arg2, arg3, new_arg2;

	if (!(iss >> arg2)) {
		std::string errorMsg = ":" + IRCname + " " + IRCerror::ERR_NEEDMOREPARAMS + \
    	" " + client->getNickname() + " " + arguments[0] + " :Not enough parameters\r\n";
		server.sendToClient(client->getNickname(), errorMsg);
		return false;
	}
	arg2 = lowerCaseChannelName(arg2);
	arguments.push_back(arg2);

	/*checking for a password and adding them to a vector*/
	if (!(iss >> arg3))
		return true;
	arguments.push_back(arg3);
	return true;
}

/*--------Function to change the format of the letters to the lowercase-----------*/
std::string CommandHandler::lowerCaseChannelName(std::string channelname) {

	if (channelname[0] == '#') {
		for (char &c : channelname) {
			c = std::tolower(c);
		}
	}
	return channelname;
}

/*------Parsing KICK command------*/
bool CommandHandler::handleKick(std::istringstream &iss, std::vector<std::string> &arguments) {
	std::string channel, user, reason;

	if (!(iss >> channel) || std::string("#!&+").find(channel[0]) == std::string::npos)
		return false;

	channel = lowerCaseChannelName(channel);
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
		return true;
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
