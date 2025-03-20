
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
	_setupServer();

}

/*---------------------------*/
/*  Server Class destructor  */
/*---------------------------*/
Server::~Server( void ) {

	std::cout << PURPLE
			  << "Server shutdown..."
			  << RESET << std::endl;

}

/*----------------*/
/*  Setup Server  */
/*----------------*/
void	Server::_setupServer( void ) {

	std::cout << BLUE
			  << "Setup Server..."
			  << RESET << std::endl;

	// Creating socket
	_serverSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( _serverSocket < 0 )
		throw std::runtime_error( "Error creating socket" );

	// Setting the address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons( IRCport );
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Binding socket
	if ( bind( _serverSocket, ( struct sockaddr* )&serverAddress, sizeof( serverAddress ) ) < 0 ) {
		close( _serverSocket );
		throw std::runtime_error( "Error binding socket" );
	}

	// Listening to the assigned socket
	if ( listen( _serverSocket, 5 ) < 0 ) {
		close( _serverSocket );
		throw std::runtime_error( "Error listening on socket" );
	}

	std::cout << BLUE
			  << "Server is ready and listening on port: "
			  << RESET
			  << CYAN
			  << IRCport << "..."
			  << RESET << std::endl;

}

/*------------------------------------*/
/*  Handle connections to the Server  */
/*------------------------------------*/
void	Server::_handleConnection( void ) {

	// Accepting connection request
	_clientSocket = accept( _serverSocket, nullptr, nullptr );
	if ( _clientSocket < 0 )
		throw std::runtime_error( "Error accepting connection" );

	std::cout << GREEN
			  << "New client connected"
			  << RESET << std::endl;

}

/*----------------------------------*/
/*  Handle data sent to the Server  */
/*----------------------------------*/
void	Server::_handleData( void ) {

	// Recieving data
	char buffer[1024] = { 0 };
	recv( _clientSocket, buffer, sizeof(buffer), 0 );

	std::cout << GREEN
			  << "Message from client: "
			  << RESET
			  << YELLOW
			  << buffer
			  << RESET << std::endl;

}

/*------------------*/
/*  Run the Server  */
/*------------------*/
void	Server::run( void ) {

	std::cout << BLUE
			  << "Server is running..."
			  << RESET << std::endl;

	_handleConnection();
	_handleData();

	// Close the socket
	close(_serverSocket);

}
