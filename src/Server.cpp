
#include "../include/Server.hpp"

/*----------------------------*/
/*  Server Class constructor  */
/*----------------------------*/
Server::Server( int port, const std::string& password ) {

	// Check port
	if ( port != IRCport )
		throw std::runtime_error( "ERROR: Port is incorrect! Use 6667 for IRC connection." );

	// Check password
	if ( password.empty() || password.size() < 3 )
		throw std::runtime_error( "ERROR: Password must contain no less than 3 characters." );

	_password = password;
	_serverFD = -1;
	_setupServer();

}

/*---------------------------*/
/*  Server Class destructor  */
/*---------------------------*/
Server::~Server( void ) {

	stop();

}

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
	if ( setsockopt( _serverFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof( opt ) ) )
		throw std::runtime_error( "Failed to set socket options" );

	// Setting the address
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons( IRCport );
	_serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Binding socket
	if ( bind( _serverFD, ( struct sockaddr* )&address, sizeof( address ) ) < 0 )
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
				  << RERSET << std::endl;
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

void	Server::_disconnectClient( int fd, const std::string& reason ) {

	Client* client = getClient( fd );
	if ( !client ) return ;

	// Remove from all channels
	for ( std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it )
		it->second->removeClient( client );

	// Remove from nickname map
	if ( client->isRegistered() )
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

void	Server::_processInputBuffer( Client* client ) {

	std::string& buffer = client->getBuffer();
	size_t	pos;

	while ( ( pos = buffer.find("\r\n") ) != std::string::npos ) {
		std::string command = buffer.substr( 0, pos );
		buffer.erase( 0, pos + 2 );

		if ( !command.empty() ) {
			std::cout << YELLOW
					  << "CMD [" << client->getFd() << "]: " << command
					  << RESET << std::endl;
			commandHandler->handleCommand( client, command );
		}
	}

}

/*-----------------------*/
/*  Handle Client data   */
/*-----------------------*/
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
	client->appendToBuffer( buffer );

	processInputBuffer( client );

	////////////////////////////////////////////////////////////////////////
	// for debugging ////////-------- delete after debugging -------////////
	////////////////////////////////////////////////////////////////////////
	std::cout << BLUE
			  << "Message from client: "
			  << RESET
			  << YELLOW
			  << buffer
			  << RESET << std::endl;
	////////////////////////////////////////////////////////////////////////

}

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
	}

	if ( _pollFDs[i].revents & ( POLLHUP | POLLERR | POLLNVAL ) )
		_disconnectClient( _pollFDs[i].fd, "Connection error" );

}

/*--------------------*/
/*  Start the Server  */
/*--------------------*/
void	Server::start( void ) {

	std::cout << BLUE
			  << "Server started on port: " << IRCport
			  << RESET << std::endl;

	while ( true ) { _handleConnections(); }

}

/*-------------------*/
/*  Stop the Server  */
/*-------------------*/
void	Server::stop( void ) {

	std::cout << BLUE
			  << "Server shutdown..."
			  << RESET << std::endl;

	// Close the socket
	close(_serverFD);

}

/*-----------------------*/
/*  Get Server password  */
/*-----------------------*/
const std::string&	Server::getPassword( void ) const { return _password; }

Client*	Server::getClient( int fd ) const {

	std::map<int, Client*>::const_iterator it = _clientsFD.find( fd );
	return it != _clientsFD.end() & it->second : NULL;

}

Client*	Server::getClient( const std::string& nickname ) const {

	std::map<std::string, Client*>::const_iterator it = _clientsNick.find( nickname );
	return it != _clientsNick.end() & it->second : NULL;

}

Channel*	Server::getChannel( const std::string& name ) const {

	std::map<std::string, Channel*>::const_iterator it = _channels.find( fd );
	return it != _channels.end() & it->second : NULL;

}

Channel*	Server::createChannel( const std::string& name, Client* creator, const std::string& key ) {



}
