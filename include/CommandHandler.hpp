/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 16:21:57 by ryusupov          #+#    #+#             */
/*   Updated: 2025/03/25 17:18:36 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP
#include <iostream>
#include <unordered_map>
#include <sstream>

#include "Client.hpp"
#include "IClient.hpp"

class CommandHandler{

	private:
		std::unordered_map<int, Client*> clients;

	public:
		void handleCommand(Client *client, const std::string &command);
		bool NickNameTaken(std::string &nickname);
		static void SendMessage(IClient *client, const std::string &msg);
		void SendError(Client *client, const std::string &msg);
};

#endif
