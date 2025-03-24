/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_c_h.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:21:57 by ryusupov          #+#    #+#             */
/*   Updated: 2025/03/24 17:59:48 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_C_H_HPP
#define CLIENT_C_H_HPP
#include <iostream>
#include <unordered_map>
#include <sstream>
#include "client.hpp"

class CommandHandler {

	private:
		Server &server;

	public:
		CommandHandler(Server& srv);
		void Handle_client_command(Client *client, const std::string &command);
		bool NickNameTaken(std::string &nickname);
		void SendMessage(Client *client, const std::string &msg);
		void SendError(Client *client, const std::string &msg);
		Client *findClient(std::string &nickname);
};

#endif
