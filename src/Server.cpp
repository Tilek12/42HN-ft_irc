
#include "../include/Server.hpp"

////////////////////////////////////////////////////////////////////////////////
// ====================  Class Constructor and Destructor  ================== //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------*/
/*  Server Class constructor  */
/*----------------------------*/
Server::Server( int port, const std::string& password ) :
	_password( password ),
	_serverFD( -1 ),
	_isRunning( false ),
	_commandHandler( new CommandHandler( *this ) ) {

	// Check port
	if ( port != IRCport )
		throw std::runtime_error( "ERROR: Port is incorrect! Use 6667 for IRC connection." );

	// Check password
	if ( password.empty() || password.size() < 3 )
		throw std::runtime_error( "ERROR: Password must contain no less than 3 characters." );

	// Set Server Creation time
	_setCreationTime();

	// Setup settings and run Server
	_setupServer();

}

/*---------------------------*/
/*  Server Class destructor  */
/*---------------------------*/
Server::~Server( void ) { stop(); }

////////////////////////////////////////////////////////////////////////////////
// ========================  Core Server operations  ======================== //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------*/
/*  Set Server Creation Time  */
/*----------------------------*/
void	Server::_setCreationTime( void ) {

	time_t now = time( nullptr );
	_creationTime = ctime( &now );

}

/*----------------*/
/*  Setup Server  */
/*----------------*/
void	Server::_setupServer( void ) {

	// Print setup message
	std::cout << BLUE
			  << "Setup Server..."
			  << RESET << std::endl;

	// Creating socket
	_serverFD = socket( AF_INET, SOCK_STREAM, 0 );
	if ( _serverFD < 0 )
		throw std::runtime_error( "Failed to create socket" );

	// Set socket options
	int opt = 1;
	if ( setsockopt( _serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt ) ) )
		throw std::runtime_error( "Failed to set socket options" );

	// Setting the address
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons( IRCport );
	_serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Binding socket
	if ( bind( _serverFD, ( struct sockaddr* )&_serverAddress, sizeof( _serverAddress ) ) < 0 )
		throw std::runtime_error( "Failed to bind socket" );

	// Listening to the assigned socket
	if ( listen( _serverFD, SOMAXCONN ) < 0 )
		throw std::runtime_error( "Failed to listen on socket" );

	// Set non-blocking mode
	if ( fcntl( _serverFD, F_SETFL, O_NONBLOCK ) < 0 )
		throw std::runtime_error( "Failed to set non-blocking mode" );

	// Setup Poll
	pollfd serverPollFD;
	serverPollFD.fd = _serverFD;
	serverPollFD.events = POLLIN;
	_pollFDs.push_back( serverPollFD );

}

/*----------------------------------------------*/
/*  Accept new Client connection to the Server  */
/*----------------------------------------------*/
void	Server::_acceptNewConnection( void ) {

	// Accepting connection request
	sockaddr_in clientAddress;
	socklen_t clientLen = sizeof( clientAddress );
	int fd = accept( _serverFD, ( struct sockaddr* )&clientAddress, &clientLen );
	if ( fd < 0 ) {
		std::cerr << RED
				  << "Failed to accept connection"
				  << RESET << std::endl;
		return;
	}

	// Set non-blocking mode
	if ( fcntl( fd, F_SETFL, O_NONBLOCK ) < 0 ) {
		close( fd );
		std::cerr << RED
				  << "Failed to set client non-blocking"
				  << RESET << std::endl;
	}

	// Create new Client
	Client* newClient = new Client( fd, inet_ntoa( clientAddress.sin_addr ) );
	newClient->setSocketFd( fd );
	_clients[fd] = newClient;

	// Setup Poll
	pollfd newPollfd;
	newPollfd.fd = fd;
	newPollfd.events = POLLIN;
	_pollFDs.push_back( newPollfd );

	// Print new connection message
	std::cout << GREEN
			  << "New connection from " << newClient->getHostname()
			  << " (fd: " << fd << ")"
			  << RESET << std::endl;

}

/*-------------------------------------*/
/*  Disconnect Client from the Server  */
/*-------------------------------------*/
void	Server::_disconnectClient( int fd, const std::string& reason ) {

	// Check for Client existance
	if ( !isClientExist( fd ) )
		return;

	// Temp values
	Client* client = getClient( fd );
	std::string nickname = client->getNickname();

	// Remove Client from all channels
	// auto channelsCopy = _channels;
	// for ( const auto& [name, channel] : channelsCopy ) {
	// 	if ( channel->isUser( nickname ) )
	// 		channel->removeUser( nickname );

	// 	if ( channel->isOperator( nickname ) )
	// 		channel->removeOperator( nickname );

	// 	if ( channel->isInvited( nickname ) )
	// 		channel->removeInvitedUser( nickname );
	// }

	// Remove from poll list
	for ( auto it = _pollFDs.begin(); it != _pollFDs.end(); ++it ) {
		if ( it->fd == fd ) {
			_pollFDs.erase( it );
			break;
		}
	}

	// Delete Client
	close( fd );
	_clients.erase( fd );
	delete client;

	// Print disconnection message
	std::cout << PURPLE << "Client disconnected (fd: " << fd << ")";
	if ( !reason.empty() ) std::cout << " - Reason: " << reason;
	std::cout << RESET << std::endl;

}

/*----------------------------*/
/*  Get Server Creation Time  */
/*----------------------------*/
const std::string& Server::getCreationTime( void ) const { return _creationTime; }

/*---------------------------------------*/
/*  Define processClientBuffer function  */
/*---------------------------------------*/
void	Server::_processClientBuffer( Client* client ) {

	std::string& buffer = client->getBuffer();
	size_t	pos;
	while ( ( pos = buffer.find("\r\n") ) != std::string::npos ) {
		std::string message = buffer.substr( 0, pos );
		buffer.erase( 0, pos + 2 ); // Remove processed message
		if ( !message.empty() ) {
			std::cout << YELLOW
					  << "CMD [" << client->getSocketFd() << "]: " << message
					  << RESET << std::endl;
			_arguments = _commandHandler->parseCommand(message);
			if (_arguments.empty())
				std::cout << "Error: Unknown command or invalid command!" << std::endl;
		}

//////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  FOR TESTING!!! DELETE!!!  ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
		int fd = client->getSocketFd();
		std::string localhost = "127.0.0.1";
		std::string host = client->getHostname();
		std::string user = client->getUsername();
		std::string nick = client->getNickname();
		// std::string time = getCreationTime();
		if ( message == "CAP LS" )
			sendToClient( fd, "CAP * LS :multi-prefix away-notify extended-join" );
		else if ( message == "PING :127.0.0.1")
			sendToClient( fd, "PONG :127.0.0.1" );
		else if ( message == "PING " + user + " TRLnet" )
			sendToClient( fd, "PONG :" + user + " TRLnet" );
		else if ( message == "CAP REQ :multi-prefix away-notify extended-join" )
			sendToClient( fd, "CAP * ACK :multi-prefix away-notify extended-join" );
		else if ( message.find("USER") != std::string::npos ) {
			// 001 RPL_WELCOME
			sendToClient( fd, ":" + IRCname + " 001 " + nick +
							" :Welcome to the IRC Network " + nick + "!" + user + "@" + host );
			// 002 RPL_YOURHOST
			sendToClient( fd, ":" + IRCname + " 002 " + nick +
							" :Your host is " + IRCname + ", running version 1.0" );
			// 003 RPL_CREATED
			sendToClient( fd, ":" + IRCname + " 003 " + nick +
							" :This server was created " + _creationTime );
			// 004 RPL_MYINFO
			sendToClient( fd, ":" + IRCname + " 004 " + nick +
							" " + IRCname + " 1.0 +aiow +ntklovb" );

			// MOTD ( Message of the Day )
			sendToClient( fd, ":" + IRCname + " 375 " + nick + " :- " + IRCname + " Message of the Day -" );
			sendToClient( fd, ":" + IRCname + " 372 " + nick + " :- Welcome to our server!" );
			sendToClient( fd, ":" + IRCname + " 376 " + nick + " :End of /MOTD command" );

		}
		else if ( message.find( "LAGCHECK" ) != std::string::npos ) {
			// std::string reply = "NOTICE " + client->getNickname() + " :LAGCHECK_REPLY " +
			// 					message.substr( message.find( "LAGCHECK" ) + 9 );
			// sendToClient( fd, reply );
			std::string reply = "PONG :" + message.substr( message.find( "LAGCHECK" ) + 9 );
			sendToClient( fd, reply );
		}
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

	}

}

/*----------------------*/
/*  Handle Client data  */
/*----------------------*/
void	Server::_handleClientData( int fd ) {

	// Recieving data
	char buffer[1024];
	ssize_t bytesRead = recv( fd, buffer, sizeof( buffer ) - 1, 0 );
	if ( bytesRead <= 0 ) {
		_disconnectClient( fd, bytesRead == 0 ? "" : "Read error" );
		return;
	}

	buffer[bytesRead] = '\0';
	Client* client = getClient( fd );
	client->appendToBuffer( buffer, bytesRead );

	// Handle complete messages
	_processClientBuffer( client );

}

/*------------------------------------*/
/*  Handle Connections to the Server  */
/*------------------------------------*/
void	Server::_handleConnections( void ) {

	if ( poll( &_pollFDs[0], _pollFDs.size(), -1 ) < 0 ) {
		if ( errno != EINTR )
			throw std::runtime_error( "Poll error!" );
		return;
	}

	for ( size_t i = 0; i < _pollFDs.size(); i++ ) {
		if ( _pollFDs[i].revents & POLLIN ) {
			if ( _pollFDs[i].fd == _serverFD )
				_acceptNewConnection();
			else
				_handleClientData( _pollFDs[i].fd );
		}

		if ( _pollFDs[i].revents & ( POLLHUP | POLLERR | POLLNVAL ) )
			_disconnectClient( _pollFDs[i].fd, "Connection error" );
	}

}

/*--------------------*/
/*  Start the Server  */
/*--------------------*/
void	Server::start( void ) {

	// Print start message
	std::cout << BLUE
			  << "Server started on port: " << IRCport
			  << RESET << std::endl;

	// Start Server
	_isRunning = true;
	while ( _isRunning )
		_handleConnections();

}

/*-------------------*/
/*  Stop the Server  */
/*-------------------*/
void	Server::stop( void ) {

	// Print shutdoen message
	std::cout << BLUE
			  << "Server shutdown..."
			  << RESET << std::endl;

	// Set running to false
	_isRunning = false;

	// Disconnect all clients
	auto clientsCopy = _clients;
	for ( const auto& [fd, client] : clientsCopy )
		_disconnectClient( fd, "Server shutdown" );

	// Cleanup channels
	auto channelsCopy = _channels;
	for ( const auto& [name, channel] : channelsCopy )
		delete channel;
	_channels.clear();

	// Close the Server socket
	if ( _serverFD != -1 ) {
		close( _serverFD );
		_serverFD = -1;
	}

	// Clear poll fds
	_pollFDs.clear();

	// Delete CommandHandler object
	delete _commandHandler;

}

/*-----------------------*/
/*  Get Server password  */
/*-----------------------*/
const std::string&	Server::getPassword( void ) const { return _password; }

/*-----------------------*/
/*  Get isRunning value  */
/*-----------------------*/
bool	Server::getIsRunning( void ) { return _isRunning; }

/*-----------------------*/
/*  Set isRunning value  */
/*-----------------------*/
void	Server::setIsRunning( bool value ) { _isRunning = value; }

/*-----------------------------------*/
/*  Get Command and argument values  */
/*-----------------------------------*/
std::vector<std::string>	Server::getArguments( void ) { return _arguments; }

////////////////////////////////////////////////////////////////////////////////
// ===========================  Client Management  ========================== //
////////////////////////////////////////////////////////////////////////////////

/*------------------*/
/*  Add new Client  */
/*------------------*/
void	Server::addClient( Client* client ) {

	if ( !isClientExist( client->getSocketFd() ) )
		_clients[client->getSocketFd()] = client;

}

/*--------------------*/
/*  Get Client by fd  */
/*--------------------*/
Client*	Server::getClient( int fd ) const {

	if ( _clients.empty() )
		return nullptr;

	auto it = _clients.find( fd );
	return it != _clients.end() ? it->second : nullptr;

}

/*--------------------------*/
/*  Get Client by nickname  */
/*--------------------------*/
Client*	Server::getClient( const std::string& nickname ) const {

	if ( _clients.empty() )
		return nullptr;

	for ( const auto& [fd, client] : _clients ) {
		if ( client->getNickname() == nickname )
			return client;
	}

	return nullptr;

}

/*--------------------------------------*/
/*  Check by nickname if Client exists  */
/*--------------------------------------*/
bool	Server::isClientExist( const std::string& nickname ) {

	return getClient( nickname ) != nullptr;

}

/*--------------------------------*/
/*  Check by fd if Client exists  */
/*--------------------------------*/
bool	Server::isClientExist( int fd ) {

	return getClient( fd ) != nullptr;

}

////////////////////////////////////////////////////////////////////////////////
// ==========================  Channel Management  ========================== //
////////////////////////////////////////////////////////////////////////////////

/*-------------------*/
/*  Add new Channel  */
/*-------------------*/
void	Server::addChannel( Channel* channel ) {

	if ( !isChannelExist( channel->getName() ) )
		_channels[channel->getName()] = channel;

}

/*---------------*/
/*  Get Channel  */
/*---------------*/
Channel*	Server::getChannel( const std::string& name ) const {

	auto it = _channels.find( name );
	return it != _channels.end() ? it->second : nullptr;

}

/*----------------------*/
/*  Create new Channel  */
/*----------------------*/
Channel*	Server::createChannel( const std::string& name ) {

	Channel* channel = new Channel( name );
	_channels[name] = channel;
	return channel;

}

/*------------------*/
/*  Remove Channel  */
/*------------------*/
void	Server::removeChannel( const std::string& name ) {

	// Check for Channel existance
	if ( !isChannelExist( name ) )
		return;

	// Delete Channel
	_channels.erase( name );
	Channel* channel = getChannel( name );
	delete channel;

}

/*---------------------------*/
/*  Check if Channel exists  */
/*---------------------------*/
bool	Server::isChannelExist( const std::string& name ) {

	return getChannel( name ) != nullptr;

}

////////////////////////////////////////////////////////////////////////////////
// ===============================  Messaging  ============================== //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------*/
/*  Send message to Client defined by fd  */
/*----------------------------------------*/
void	Server::sendToClient( int fd, const std::string& message ) {

	if ( !isClientExist( fd ) )
		return;

	std::string reply = message + "\r\n";

	if ( send( fd, reply.c_str(), reply.length(), 0 ) < 0 )
		_disconnectClient( fd, "Send error" );

}

/*----------------------------------------------*/
/*  Send message to Client defined by nickname  */
/*----------------------------------------------*/
void	Server::sendToClient( const std::string& nickname, const std::string& message ) {

	if ( !isClientExist( nickname ) )
		return;

	Client* client = getClient( nickname );

	sendToClient( client->getSocketFd(), message );

}

/*----------------------------------------------*/
/*  Send Error message to Client defined by fd  */
/*----------------------------------------------*/
void	Server::sendError( int fd, const std::string& errorCode, const std::string& message ) {

	std::string	nickname = "*";
	if ( _clients.count(fd) && !_clients[fd]->getNickname().empty() )
		nickname = _clients[fd]->getNickname();

	std::string errorReply = ":" + IRCname + " " + errorCode + " " + nickname + " " + message;

	sendToClient( fd, errorReply );

	// Log the error
	std::cerr << RED << "[ERROR] Sent to fd=" << fd
			  << " (" << nickname << "): " << errorCode << " " << message
			  << RESET << std::endl;

}

/*--------------------------------*/
/*  Broadcast message in Channel  */
/*--------------------------------*/
void	Server::broadcastMessage( const std::string& channelName, const std::string& message ) {

	Channel* channel = getChannel( channelName );
	if ( !channel ) return;

	std::unordered_set<std::string> recipients;

	for ( const auto& group : { channel->getUsers(),
								channel->getOperators(),
								channel->getInvitedUsers() } )
	{
		recipients.insert( group.begin(), group.end() );
	}

	for ( const auto& [fd, client] : _clients ) {
		if ( recipients.count( client->getNickname() ) )
			sendToClient( fd, message );
	}

}
