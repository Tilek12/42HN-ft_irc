/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:44:20 by ryusupov          #+#    #+#             */
/*   Updated: 2025/04/21 16:29:42 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

// Client::Client( std::string nickName ) {}

Client::Client(int fd, std::string host) : socket_fd(fd),
										   nickname( "" ),
										   username( "" ),
										   realname( "" ),
										   hostname(host),
										   isRegistered(false),
										   isAuthenticated(false),
										   userReceived(false),
										   buffer( "" ),
										   sendBuffer( "" ) {
	//intializing values
}

/*Getters*/
int Client::getSocketFd() const {
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
void	Client::setSocketFd(int fd) {
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

void	Client::markUserReceived() {
	userReceived = true;
}

/*Destructor*/
Client::~Client() {
	//
 }


// ===== BUFFER MANAGEMENT =====
void Client::appendToBuffer(const char* data, size_t len) {
    buffer.append(data, len);
}

void Client::clearBuffer() {
    buffer.clear();
}

std::string& Client::getBuffer() {
    return buffer;
}

bool Client::hasPendingData() const {
    return !sendBuffer.empty();
}

void Client::queueMessage(const std::string& message) {
    sendBuffer += message;
    if (sendBuffer.find("\r\n") == std::string::npos) {
        sendBuffer += "\r\n";
    }
}

std::string Client::getNextMessage() {
    size_t pos = sendBuffer.find("\r\n");
    if (pos == std::string::npos) return "";

    std::string message = sendBuffer.substr(0, pos + 2);
    sendBuffer.erase(0, pos + 2);
    return message;
}

/////////////////////////  NEW FUNCTION  ///////////////////////////////
bool	Client::getIsAuthenticated() const { return isAuthenticated; }

/////////////////////////  NEW FUNCTION  ///////////////////////////////
void	Client::setIsAuthenticated(bool status) { isAuthenticated = status; }
