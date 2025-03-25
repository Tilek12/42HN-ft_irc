
#pragma once

#include <string>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <netinet/in.h>
#include "colors.hpp"
#include "IServer.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "CommandHandler.hpp"

/*-------------------*/
/*  Define IRC port  */
/*-------------------*/
const int IRCport = 6667;

/*-----------------------*/
/*  Define Server class  */
/*-----------------------*/
class Server : public IServer {

private:

	std::string							_password;		// Password for Server
	int									_serverFD;		// file descriptor for Server socket
	struct sockaddr_in					_serverAddress;	// Server socket address
	std::vector<pollfd>					_pollFDs;		// List of file descriptors to monitor
	std::unordered_map<int, Client*>	_clientsFD;		// Map of client FDs to Client pointers
	std::map<std::string, Client*>		_clientsNick;	// Map of client Nickname to Client
	std::map<std::string, Channel*>		_channels;		// Map of channel names to Channel pointers

	void	_setupServer( void );
	void	_handleConnections( void );
	void	_acceptNewConnection( void );
	void	_handleClientData( int fd );
	void	_disconnectClient( int fd, const std::string& reason = "" );

public:

	Server( int port, const std::string& password );
	~Server( void );
	Server( const Server& other ) = delete;
	Server& operator=( const Server&  other ) = delete;

	void		start( void ) override;
	void		stop( void ) override;
	std::string	getPassword( void ) override;
	// void		broadcastMessage( const std::string& channel, const std::string& message );

};
