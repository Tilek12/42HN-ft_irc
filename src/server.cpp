
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
	struct sockaddr_in	address;
	address.sin_family = AF_INET;
	address.sin_port = htons( IRCport );
	address.sin_addr.s_addr = INADDR_ANY;

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
	_fds.push_back( serverPollFD );

}

/*----------------------------------------------*/
/*  Accept new Client connection to the Server  */
/*----------------------------------------------*/
void	Server::_acceptNewConnection( void ) {

	// Accepting connection request
	_clientSocket = accept( _serverFD, nullptr, nullptr );
	if ( _clientSocket < 0 )
		throw std::runtime_error( "Error accepting connection" );

	std::cout << GREEN
			  << "New client connected..."
			  << RESET << std::endl;

}

/*-----------------------*/
/*  Handle Client data   */
/*-----------------------*/
void	Server::_handleClientData( int clientFD ) {

	// Recieving data
	char buffer[1024] = { 0 };
	recv( _clientSocket, buffer, sizeof(buffer), 0 );

	std::cout << BLUE
			  << "Message from client: "
			  << RESET
			  << YELLOW
			  << buffer
			  << RESET << std::endl;

}

void	Server::_handleConnections( void ) {

	if ( poll( &_fds[0], _fds.size(), -1 ) < 0 ) {
		if ( errno != EINTR )
			throw std::runtime_error( "Poll error!" );
		return;
	}

	for ( size_t i = 0; i < _fds.size(); i++ ) {
		if ( _fds[i].revents & POLLIN ) {
			if ( _fds[i].fd == _serverFD )
				_acceptNewConnection();
			else
				_handleClientData( _fds[i].fd );
		}
	}

	if ( _fds[i].revents & ( POLLHUP | POLLERR | POLLNVAL ) )
		_disconnectClient( _fds[i].fd, "Connection error" );

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

/*------------------*/
/*  Stop the Server  */
/*------------------*/
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
std::string	Server::getPassword( void ) { return _password; }
