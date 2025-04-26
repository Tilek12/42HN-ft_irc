
#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <fcntl.h>
#include <poll.h>
#include <csignal>
#include "colors.hpp"
#include "IServer.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "client_c_h.hpp"
#include "IRCreply.hpp"
#include "IRCerror.hpp"

/*------------------------------------*/
/*  Define Global constant variables  */
/*------------------------------------*/
const int IRCport = 6667;
const std::string IRCname = "TRLnet";

/*----------------------------*/
/*  Add CommandHandler class  */
/*----------------------------*/
class CommandHandler;

/*-----------------------*/
/*  Define Server class  */
/*-----------------------*/
class Server : public IServer {

private:

	// std::string						_creationTime;		// Server Creation time
	std::string						_password;			// Password for Server
	int								_serverFD;			// file descriptor for Server socket
	struct sockaddr_in				_serverAddress;		// Server socket address
	std::vector<pollfd>				_pollFDs;			// List of file descriptors to monitor
	std::map<int, Client*>			_clients;			// Map of client FDs to Client pointers
	std::map<std::string, Channel*>	_channels;			// Map of channel names to Channel pointers
	bool							_isRunning;			// Server running check
	CommandHandler*					_commandHandler;	// Local Command Handler object
	std::vector<std::string>		_arguments;			// Parsed command and arguments

	Server( const Server& other ) = delete;
	Server& operator=( const Server&  other ) = delete;

	// void	_setCreationTime( void );
	void	_setupServer( void );
	void	_handleConnections( void );
	void	_acceptNewConnection( void );
	void	_handleClientData( int fd );
	void	_processClientBuffer( Client* client );

	public:

	// Class Constructor and Destructor
	Server( int port, const std::string& password );
	~Server( void );

	// Core server operations
	void						start( void ) override;
	void						stop( void ) override;
	bool						validatePassword( const std::string& pass ) const override;
	// const std::string& 			getCreationTime( void ) const override;
	bool						getIsRunning( void ) override;
	void						setIsRunning( bool value ) override;
	std::vector<std::string>	getArguments( void ) override;

	// Client management
	void	addClient( Client* client ) override;
	Client*	getClient( int fd ) const override;
	Client*	getClient( const std::string& nickname ) const override;
	bool	isClientExist( const std::string& nickname ) override;
	bool	isClientExist( int fd ) override;
	bool	checkClientAuthentication( Client* client ) override;
	void	disconnectClient( int fd, const std::string& reason ) override;
	void	removeClientFromChannels( Client* client ) override;

	// Channel management
	void							addChannel( Channel* channel ) override;
	Channel*						getChannel( const std::string& name ) const override;
	std::map<std::string, Channel*>	getChannels( void ) override;
	Channel*						createChannel( const std::string& name ) override;
	void							removeChannel( const std::string& name ) override;
	bool							isChannelExist( const std::string& name ) override;
	std::vector<Channel*>			getClientChannels( Client* client ) override;

	// Messaging
	void	sendToClient( int fd, const std::string& message ) override;
	void	sendToClient( const std::string& nickname, const std::string& message ) override;
	void	sendError( int fd, const std::string& errorCode, const std::string& message ) override;
	void	broadcastMessage( Client* client,
							  const std::string& channelName,
							  const std::string& message ) override;
	void	broadcastMsgInClientChannels( Client* client, const std::string& message ) override;

};

void	serverCmdHandler( Server* server, Client* client );
