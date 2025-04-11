/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryusupov <ryusupov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:03:06 by ryusupov          #+#    #+#             */
/*   Updated: 2025/04/08 19:58:54 by ryusupov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#define CLIENT_HPP
#include "IClient.hpp"
#include <iostream>
#include <map>

class Client : public IClient{

	private:
		int socket_fd;			//file descriptor of client socket connection
		std::string nickname;	//clinet's nickname
		std::string username;	//clinet's username
		std::string realname;	//clinet's real name
		std::string hostname;	//client's hostname
		bool isRegistered;		//the flag to check is the client has completed registration process
		bool userReceived;
		std::string buffer;		// Buffer for recieved message
		std::string sendBuffer;	// Buffer for output message

		public:
			/*Costructor to initialize*/
			Client(int fd, std::string host);

			/*Getters for nickname, username, realname and hostname*/
			int	getSocketFd() const override;
			std::string getNickname() const override;
			std::string getUsername() const override;
			std::string getRealname() const override;
			std::string getHostname() const override;
			bool		getIsResgistered() const override;
			void		markUserReceived();

			/*Setters*/
			void	setSocketFd(int fd) override;
			void	setNickname(std::string &nickname) override;
			void	setUsername(std::string &username) override;
			void	setRealname(std::string &realname) override;
			void	setHostname(std::string &hostname) override;
			void	setIsRegistered(bool status) override;


			// Buffer management
			void			appendToBuffer(const char* data, size_t len) override;
			void			clearBuffer() override;
			std::string&	getBuffer() override;
			bool			hasPendingData() const override;
			void			queueMessage(const std::string& message) override;
			std::string		getNextMessage() override;


			/*Destructor*/
			~Client();
};

// class Server {
//     public:
//         std::map<std::string, Client*> clients; // Map by nickname instead of int

// 		void addClient(Client* client) {
// 			if (!client->getNickname().empty()) {
// 				clients[client->getNickname()] = client;
// 			}
// 		}

// 		void printClients() {
// 			std::cout << "Current Clients in Server:" << std::endl;
// 			for (auto& c : clients) {
// 				std::cout << "Nickname: " << c.first << " -> FD: " << c.second->getSockedFd() << std::endl;
// 			}
// 		}

//         Client* findClient(const std::string& nickname) {
// 			for (auto &nick : clients) {
// 				if (nick.second->getNickname() == nickname) {
// 					return nick.second;
// 				}
// 			}
// 			return (nullptr);
//         }
//     };

#endif
