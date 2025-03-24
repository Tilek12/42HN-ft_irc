/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:03:06 by ryusupov          #+#    #+#             */
/*   Updated: 2025/03/24 19:57:19 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "IClient.hpp"
#include <iostream>
#include <map>

class Client{

	private:
		int socket_fd;			//file descriptor of client socket connection
		std::string nickname;	//clinet's nickname
		std::string username;	//clinet's username
		std::string realname;	//clinet's real name
		std::string hostname;	//client's hostname
		bool isRegistered;		//the flag to check is the client has completed registration process

		public:
			/*Costructor to initialize*/
			Client(int fd, std::string host);

			/*Getters for nickname, username, realname and hostname*/
			int	getSockedFd() const;
			std::string getNickname() const;
			std::string getUsername() const;
			std::string getRealname() const;
			std::string getHostname() const;
			bool		getIsResgistered() const;

			/*Setters*/
			void	setSockedFd(int fd);
			void	setNickname(std::string &nickname);
			void	setUsername(std::string &username);
			void	setRealname(std::string &realname);
			void	setHostname(std::string &hostname);
			void	setIsRegistered(bool status);


			/*Destructor*/
			~Client();

};

class Server {
    public:
        std::map<std::string, Client*> clients; // Map by nickname instead of int

		void addClient(Client* client) {
			if (!client->getNickname().empty()) {
				clients[client->getNickname()] = client;
			}
		}

		void printClients() {
			std::cout << "Current Clients in Server:" << std::endl;
			for (auto& c : clients) {
				std::cout << "Nickname: " << c.first << " -> FD: " << c.second->getSockedFd() << std::endl;
			}
		}

        Client* findClient(const std::string& nickname) {
			for (auto &nick : clients) {
				if (nick.second->getNickname() == nickname) {
					return nick.second;
				}
			}
			return (nullptr);
        }
    };

#endif
