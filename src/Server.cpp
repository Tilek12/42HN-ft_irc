
#include "../include/Server.hpp"

////////////////////////////////////////////////////////////////////////////////
// ====================  Class constructor and destructor  ================== //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------*/
/*  Server Class constructor  */
/*----------------------------*/
Server::Server( int port, const std::string& password ) :
	_password( password),
	_serverFD( -1 ),
	_isRunning( false ),
	_commandHandler( new CommandHandler(*this) ) {

	// Check port
	if ( port != IRCport )
		throw std::runtime_error( "ERROR: Port is incorrect! Use 6667 for IRC connection." );

	// Check password
	if ( password.empty() || password.size() < 3 )
		throw std::runtime_error( "ERROR: Password must contain no less than 3 characters." );

	_setupServer();

}

/*---------------------------*/
/*  Server Class destructor  */
/*---------------------------*/
Server::~Server( void ) {

	stop();
	delete _commandHandler;

}

////////////////////////////////////////////////////////////////////////////////
// ========================  Core server operations  ======================== //
////////////////////////////////////////////////////////////////////////////////

/*----------------*/
/*  Setup Server  */
/*----------------*/
void	Server::_setupServer( void ) {

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
	int clientFD = accept( _serverFD, ( struct sockaddr* )&clientAddress, &clientLen );

	if ( clientFD < 0 ) {
		std::cerr << RED
				  << "Failed to accept connection"
				  << RESET << std::endl;
		return;
	}

	if ( fcntl( clientFD, F_SETFL, O_NONBLOCK ) < 0 ) {
		close( clientFD );
		std::cerr << RED
				  << "Failed to set client non-blocking"
				  << RESET << std::endl;
	}

	Client* newClient = new Client( clientFD, inet_ntoa( clientAddress.sin_addr ) );
	_clientsFD[clientFD] = newClient;

	pollfd newPollfd;
	newPollfd.fd = clientFD;
	newPollfd.events = POLLIN;
	_pollFDs.push_back( newPollfd );

	std::cout << GREEN
			  << "New connection from " << newClient->getHostname()
			  << " (fd: " << clientFD << ")"
			  << RESET << std::endl;

}

/*-------------------------------------*/
/*  Disconnect Client from the Server  */
/*-------------------------------------*/
void	Server::_disconnectClient( int fd, const std::string& reason ) {

	Client* client = getClient( fd );
	if ( !client ) return ;

	// Remove from all channels
	for ( std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it ) {
		std::set<std::string> users = it->second->getUsers();
		if (users.find(client->getNickname()) != users.end())
			it->second->removeUser( client->getNickname() );

	}

	// Remove from nickname map
	if ( client->getIsResgistered() )
		_clientsNick.erase( client->getNickname() );

	// Cleanup
	close( fd );
	_clientsFD.erase( fd );
	delete client;

	// Remove from poll list
	for ( std::vector<pollfd>::iterator it = _pollFDs.begin(); it != _pollFDs.end(); ++it ) {
		if ( it->fd == fd ) {
			_pollFDs.erase( it );
			break;
		}
	}

	std::cout << PURPLE
			  << "Client disconnected (fd: " << fd << ")";
	if ( !reason.empty() ) std::cout << " - Reason: " << reason;
	std::cout << RESET << std::endl;

}

/*---------------------------------------*/
/*  Define processClientBuffer fucntion  */
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
			std::cout << "size: " << _arguments.size() << std::endl;
		}
	}

}

/*----------------------*/
/*  Handle Client data  */
/*----------------------*/
void	Server::_handleClientData( int fd ) {

	// Recieving data
	char buffer[1024];
	ssize_t bytesRead = recv( fd, buffer, sizeof(buffer) - 1, 0 );

	if ( bytesRead <= 0 ) {
		_disconnectClient( fd, bytesRead == 0 ? "Client disconnected" : "Read error" );
		return;
	}

	buffer[bytesRead] = '\0';
	Client* client = getClient( fd );
	client->appendToBuffer( buffer, bytesRead );

	_processClientBuffer( client ); // Handle complete messages

	////////////////////////////////////////////////////////////////////////
	// for debugging /////// -------- delete after debugging ------- ///////
	////////////////////////////////////////////////////////////////////////
	std::cout << BLUE
			  << "Message from client: "
			  << RESET
			  << YELLOW
			  << buffer
			  << RESET << std::endl;
	////////////////////////////////////////////////////////////////////////

}

/*------------------------------------*/
/*  Handle connections to the Server  */
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

	std::cout << BLUE
			  << "Server started on port: " << IRCport
			  << RESET << std::endl;

	_isRunning = true;

	while ( _isRunning ) { _handleConnections(); }

}

/*-------------------*/
/*  Stop the Server  */
/*-------------------*/
void	Server::stop( void ) {

	std::cout << BLUE
			  << "Server shutdown..."
			  << RESET << std::endl;

	// Disconnect all clients
	for ( std::map<int, Client*>::iterator it = _clientsFD.begin(); it != _clientsFD.end(); ++it )
		_disconnectClient( it->first, "Server shutdown" );

	// Cleanup channels
	for ( std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it )
		delete it->second;
	_channels.clear();

	// Close the Server socket
	if ( _serverFD != -1 ) {
		close(_serverFD);
		_serverFD = -1;
	}

}

/*-----------------------*/
/*  Get Server password  */
/*-----------------------*/
const std::string&	Server::getPassword( void ) const { return _password; }

bool	Server::getIsRunning( void ) { return _isRunning; }

void	Server::setIsRunning( bool value ) { _isRunning = value; }

////////////////////////////////////////////////////////////////////////////////
// ===========================  Client management  ========================== //
////////////////////////////////////////////////////////////////////////////////

/*-----------------------------*/
/*  Define getClient function  */
/*-----------------------------*/
Client*	Server::getClient( int fd ) const {

	std::map<int, Client*>::const_iterator it = _clientsFD.find( fd );
	return it != _clientsFD.end() ? it->second : NULL;

}

/*-----------------------------*/
/*  Define getClient function  */
/*-----------------------------*/
Client*	Server::getClient( const std::string& nickname ) const {

	std::map<std::string, Client*>::const_iterator it = _clientsNick.find( nickname );
	return it != _clientsNick.end() ? it->second : NULL;

}

////////////////////////////////////////////////////////////////////////////////
// ==========================  Channel management  ========================== //
////////////////////////////////////////////////////////////////////////////////

/*------------------------------*/
/*  Define getChannel function  */
/*------------------------------*/
Channel*	Server::getChannel( const std::string& name ) const {

	std::map<std::string, Channel*>::const_iterator it = _channels.find( name );
	return it != _channels.end() ? it->second : NULL;

}

/*---------------------------------*/
/*  Define createChannel function  */
/*---------------------------------*/
Channel*	Server::createChannel( const std::string& name ) {

	Channel* channel = new Channel( name );
	_channels[name] = channel;
	return channel;

}

/*---------------------------------*/
/*  Define removeChannel function  */
/*---------------------------------*/
void	Server::removeChannel( const std::string& name ) {

	std::map<std::string, Channel*>::const_iterator it = _channels.find( name );

	if ( it != _channels.end() )
		_channels.erase( it );

}

////////////////////////////////////////////////////////////////////////////////
// ===============================  Messaging  ============================== //
////////////////////////////////////////////////////////////////////////////////

/*--------------------------------*/
/*  Define sendToClient function  */
/*--------------------------------*/
void	Server::sendToClient( int fd, const std::string& message ) {

	if ( send( fd, message.c_str(), message.length(), 0 ) < 0 )
		_disconnectClient( fd, "Send error" );

}

void Server::addClient(Client* client) {
	if (!client->getNickname().empty()) {
		_clientsNick[client->getNickname()] = client;
	}
}

/*------------------------------------*/
/*  Define broadcastMessage function  */
/*------------------------------------*/
// void	Server::broadcastMessage( const std::string& channel, const std::string& message ) {

// 	const std::map<int, Client*>& members = channel->getClients();

// 	for (std::map<int, Client*>::const_iterator it = members.begin(); it != members.end(); ++it )
// 		sendToClient( it->first, message );

// }
