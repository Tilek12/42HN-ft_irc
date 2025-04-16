/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_c_h.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/16 12:31:31 by ryusupov         ###   ########.fr       */
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
		static void SendMessage(IClient *client, const std::string &msg);
		void SendError(Client *client, const std::string &msg);

		std::vector<std::string> &getArguments();
		static std::vector<std::string> parseCommand(const std::string &command);
		static std::vector<std::string> parseSpecialCommands(const std::string &command, const std::unordered_set<std::string> &setOfCmds);
		/*Helper methods for functions*/
		static bool handlePrivMsgNotice(std::istringstream &iss, std::vector<std::string> &arguments);
		static bool handleUser(std::istringstream &iss, std::vector<std::string> &arguments);
		static bool handleTopic(std::istringstream &iss, std::vector<std::string> &arguments);
		static bool handleMode(std::istringstream &iss, std::vector<std::string> &arguments);
		static bool handleSimpleCommands(std::istringstream &iss, std::vector<std::string> &arguments);
		static bool handleKick(std::istringstream &iss, std::vector<std::string> &arguments);
		static bool handlePing(std::istringstream &iss, std::vector<std::string> &arguments);
		static bool handleCap(std::istringstream &iss, std::vector<std::string> &arguments);
		static bool handleNotice(std::istringstream &iss, std::vector<std::string> &arguments);
		static bool handlePart(std::istringstream &iss, std::vector<std::string> &args);
		void	MainCommandHandller(Client *client, std::vector<std::string> &args);
		bool 	handleNickname(Client *client, std::vector<std::string> &command);
		void	registerClient(Client *client);
		void	findTargetPrivmsg(Client *client, std::vector<std::string> &command);
		void	handleWhoCmd(Client *client, std::vector<std::string> &command);



};

#endif
