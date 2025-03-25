/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command_handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llacsivy <llacsivy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:21:57 by ryusupov          #+#    #+#             */
/*   Updated: 2025/03/25 16:23:45 by llacsivy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP
#include <iostream>
#include <unordered_map>
#include <sstream>

#include "Client.hpp"
#include "IClient.hpp"

class CommandHandler{

	private:
		std::unordered_map<int, Client*> clients;

	public:
		void Handle_command(Client *client, const std::string &command);
		bool NickNameTaken(std::string &nickname);
		static void SendMessage(IClient *client, const std::string &msg);
		void SendError(Client *client, const std::string &msg);
};

#endif
