/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_c_h.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/01 17:02:32 by ryusupov         ###   ########.fr       */
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
		void handleCommand(Client *client, const std::string &command);
		CommandHandler(Server& srv);
		bool NickNameTaken(std::string &nickname);
		static void SendMessage(IClient *client, const std::string &msg);
		void SendError(Client *client, const std::string &msg);

		static std::vector<std::string> parseCommand(const std::string &command);
		static std::vector<std::string> parseSpecialCommands(const std::string &command);
};

#endif
