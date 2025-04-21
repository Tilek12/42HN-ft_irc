/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_c_h.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/21 16:25:26 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_C_H_HPP
#define CLIENT_C_H_HPP
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <unordered_set>

#include "Client.hpp"
#include "IClient.hpp"
#include "Server.hpp"

class Server;
class CommandHandler{

	private:
		Server &server;

	public:
		void clientCmdHandler(Client *client, std::vector<std::string> &command);
		CommandHandler(Server& srv);
		bool NickNameTaken(std::string &nickname);
		static void SendMessage(const std::string &msg);
		void SendError(const std::string &msg);

		std::vector<std::string> &getArguments();
		std::vector<std::string> parseCommand(Client *client, const std::string &command);
		std::vector<std::string> parseSpecialCommands(Client *client, const std::string &command, const std::unordered_set<std::string> &setOfCmds);
		std::vector<std::string> splitTargets(std::string &target);
		/*Helper methods for functions*/
		bool	handlePrivMsgNotice(Client *client, std::istringstream &iss, std::vector<std::string> &arguments);
		bool	handleUser(Client *client, std::istringstream &iss, std::vector<std::string> &arguments);
		bool	handleTopic(Client *client, std::istringstream &iss, std::vector<std::string> &arguments);
		bool	handleMode(Client *client, std::istringstream &iss, std::vector<std::string> &arguments);
		bool	handleSimpleCommands(Client *client, std::istringstream &iss, std::vector<std::string> &arguments);
		bool	handleKick(std::istringstream &iss, std::vector<std::string> &arguments);
		bool	handlePing(std::istringstream &iss, std::vector<std::string> &arguments);
		bool	handleCap(std::istringstream &iss, std::vector<std::string> &arguments);
		bool	handleNotice(std::istringstream &iss, std::vector<std::string> &arguments);
		bool	handlePart(Client *client, std::istringstream &iss, std::vector<std::string> &args);
		void	updateNicknameInChannels(Client *client, const std::string &oldNick, const std::string &newNick, std::string msg);
		void	MainCommandHandller(Client *client, std::vector<std::string> &args);
		bool 	handleNickname(Client *client, std::vector<std::string> &command);
		void	registerClient(Client *client);
		void	findTargetPrivmsg(Client *client, std::vector<std::string> &command);
		void	handleWhoCmd(Client *client, std::vector<std::string> &command);
		bool 	handleQuit(Client *client, std::istringstream &iss, std::vector<std::string> &args);



};

#endif
