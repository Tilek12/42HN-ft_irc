
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
#include "colors.hpp"
#include "IServer.hpp"
#include "Client.hpp"
#include "Channel.hpp"

/*-------------------*/
/*  Define IRC port  */
/*-------------------*/
const int IRCport = 6667;

/*-----------------------*/
/*  Define Server class  */
/*-----------------------*/
class Server : public IServer {

private:

	std::string							_password;	// Password for Server
	int									_serverFD;	// file descriptor for Server socket
	std::vector<pollfd>					_fds;		// List of file descriptors to monitor
	std::unordered_map<int, Client*>	_clients;	// Map of client FDs to Client objects
	std::map<std::string, Channel*>		_channels;	// Map of channel names to Channel objects

	void	_setupServer( void );
	void	_handleConnections( void );
	void	_acceptNewConnection( void );
	void	_handleClientData( int clientFD );
	void	_disconnectClient( int clientFD, const std::string& reason = "" );

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
