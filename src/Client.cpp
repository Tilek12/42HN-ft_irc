/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llacsivy <llacsivy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:44:20 by ryusupov          #+#    #+#             */
/*   Updated: 2025/03/25 14:01:42 by llacsivy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd, std::string host) : socket_fd(fd), hostname(host), isRegistered(false) {
	//intializing values
}

/*Getters*/
int Client::getSockedFd() const {
	return socket_fd;
}

std::string Client::getNickname() const {
	return nickname;
}

std::string Client::getUsername() const {
	return username;
}

std::string Client::getRealname() const {
	return realname;
}

std::string Client::getHostname() const {
	return hostname;
}


bool Client::getIsResgistered() const {
	return isRegistered;
}

/*Setters*/
void	Client::setSockedFd(int fd) {
	socket_fd = fd;
}

void	Client::setNickname(std::string &nickname) {
	this->nickname = nickname;
}

void	Client::setUsername(std::string &username) {
	this->username = username;
}

void	Client::setRealname(std::string &realname) {
	this->realname = realname;
}

void	Client::setHostname(std::string &hostname) {
	this->hostname = hostname;
}

void	Client::setIsRegistered(bool status) {
	this->isRegistered = status;
}

/*Destructor*/
Client::~Client() {
	std::cout << "Client" << this->nickname << " has been disconnected!" << std::endl;
 }
