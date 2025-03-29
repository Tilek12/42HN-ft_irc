/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_c_h.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/03/28 17:20:54 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_C_H_HPP
#define CLIENT_C_H_HPP
#include <iostream>
#include <unordered_map>
#include <sstream>

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
};

#endif
