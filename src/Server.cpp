
#include "../include/Server.hpp"
#include "../include/IRCerror.hpp"

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

/*---------------------------------------*/
/*  Define processClientBuffer function  */
/*---------------------------------------*/
void	Server::_processClientBuffer( Client* client ) {

	std::string&	buffer = client->getBuffer();
	std::string		message;
	size_t			pos;

	while ( client && ( pos = buffer.find("\n") ) != std::string::npos ) {
		if ( buffer[pos - 1] == '\r' )
			message = buffer.substr( 0, pos - 1 );
		else
			message = buffer.substr( 0, pos );

		buffer.erase( 0, pos + 1 );

		if ( client && !message.empty() ) {
			std::cout << YELLOW
					  << "CMD INPUT  [" << client->getSocketFd() << "]: " << message
					  << RESET << std::endl;
			_arguments = _commandHandler->parseCommand( client, message );
			if ( _arguments.size() > 1 )
				_commandHandler->MainCommandHandller( client, _arguments );
			else if ( _arguments.size() == 1 )
				sendError( client->getSocketFd(), IRCerror::ERR_UNKNOWNCOMMAND, _arguments[0] );
			else
				return ;
		}
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
		disconnectClient( fd, bytesRead == 0 ? "" : "Read error" );
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
			disconnectClient( _pollFDs[i].fd, "Connection error" );
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
		disconnectClient( fd, "Server shutdown" );

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

/*-------------------------*/
/*  Check Server password  */
/*-------------------------*/
bool	Server::validatePassword( const std::string& pass ) const { return pass == _password; }

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

/*-------------------------------------*/
/*  Disconnect Client from the Server  */
/*-------------------------------------*/
void	Server::disconnectClient( int fd, const std::string& reason ) {

	// Check for Client existance
	if ( !isClientExist( fd ) )
		return;

	// Temp values
	Client* client = getClient( fd );
	std::string nickname = client->getNickname();

	// Remove Client from all channels
	removeClientFromChannels( client );

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
	client->clearBuffer();
	delete client;

	// Print disconnection message
	std::cout << PURPLE << "Client disconnected (fd: " << fd << ")";
	if ( !reason.empty() ) std::cout << " - Reason: " << reason;
	std::cout << RESET << std::endl;

}

/*---------------------------------*/
/*  Check password Authentication  */
/*---------------------------------*/
bool	Server::checkClientAuthentication( Client* client ) {

	if ( !client->getIsAuthenticated() ) {
		sendError( client->getSocketFd(), IRCerror::ERR_PASSWDMISMATCH, ":Password required" );
		disconnectClient( client->getSocketFd(), "No Password provided" );
		return false;
	}

	return true;

}

/*--------------------------------------------*/
/*  Remove Client from all Channels included  */
/*--------------------------------------------*/
void	Server::removeClientFromChannels( Client* client ) {

	std::string clientName = client->getNickname();

	for ( const auto& [channelName, channel] : _channels ) {
		if ( channel->isUser( clientName ) ||
				channel->isOperator( clientName ) ||
				channel->isInvitedUser( clientName ) )
			channel->removeUser( clientName );
	}

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

/*--------------------*/
/*  Get all Channels  */
/*--------------------*/
std::map<std::string, Channel*>  Server::getChannels( void ) { return _channels; }

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
	Channel* channel = getChannel( name );
	_channels.erase( name );
	delete channel;

}

/*---------------------------*/
/*  Check if Channel exists  */
/*---------------------------*/
bool	Server::isChannelExist( const std::string& name ) {

	return getChannel( name ) != nullptr;

}

/*----------------------------------------*/
/*  Get all Channels where the Client is  */
/*----------------------------------------*/
std::vector<Channel*>	Server::getClientChannels( Client* client ) {

	std::vector<Channel*>	channels;
	std::string clientName = client->getNickname();

	for ( const auto& [channelName, channel] : _channels ) {
		if ( channel->isUser( clientName ) ||
				channel->isOperator( clientName ) ||
				channel->isInvitedUser( clientName ) )
			channels.push_back( channel );
	}

	return channels;
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

	std::cout << CYAN
			  << "CMD OUTPUT [" << fd << "]: " << message
			  << RESET << std::endl;

	std::string reply = message;

	if ( message.find("\r\n")  == std::string::npos )
		reply += "\r\n";
	if ( send( fd, reply.c_str(), reply.length(), 0 ) < 0 )
		disconnectClient( fd, "Send error" );

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

	std::cerr << RED << "[ERROR] " << RESET;

	sendToClient( fd, errorReply );

}

/*--------------------------------*/
/*  Broadcast message in Channel  */
/*--------------------------------*/
void	Server::broadcastMessage( Client* client, const std::string& channelName, const std::string& message ) {

	Channel* channel = getChannel( channelName );
	if ( !channel ) return;

	std::unordered_set<std::string> recipients;

	for ( const auto& group : { channel->getUsers(),
								channel->getOperators(),
								channel->getInvitedUsers() } )
	{
		recipients.insert( group.begin(), group.end() );
	}

	recipients.erase( client->getNickname() );

	for ( const auto& [fd, client] : _clients ) {
		if ( recipients.count( client->getNickname() ) )
			sendToClient( fd, message );
	}

}

/*---------------------------------------------------------*/
/*  Broadcast message in All Channels where the Client is  */
/*---------------------------------------------------------*/
void	Server::broadcastMsgInClientChannels( Client* client, const std::string& message ) {

	std::string clientName = client->getNickname();

	for ( const auto& [channelName, channel] : _channels ) {
		if ( channel->isUser( clientName ) ||
				channel->isOperator( clientName ) ||
				channel->isInvitedUser( clientName ) )
			broadcastMessage( client, channelName, message );
	}

}
