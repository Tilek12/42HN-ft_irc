/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:21:57 by ryusupov          #+#    #+#             */
/*   Updated: 2025/03/22 16:39:36 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP
#include <iostream>
#include <unordered_map>
#include <sstream>
#include "client.hpp"

class CommandHandler {

	private:
		std::unordered_map<int, Client*> clients;

	public:
		void Handle_command(Client *client, const std::string &command);
		bool NickNameTaken(std::string &nickname);
		void SendMessage(Client *client, const std::string &msg);
		void SendError(Client *client, const std::string &msg);
};

#endif
