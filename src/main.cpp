
#include "../include/Server.hpp"

Server*	globalServer = nullptr;

void	signalHandler( int ) {

	if ( globalServer )
		globalServer->setIsRunning( false );

}

int main( int argc, char** argv ) {

	std::cout << YELLOW
			  << "==============================================" << std::endl
			  << "===========  TRLnet - IRC Network  ===========" << std::endl
			  << "==============================================" << std::endl
			  << RESET;

	if ( argc != 3 ) {
		std::cerr << RED
				  << "Usage: ./ircserv <port> <password>"
				  << RESET << std::endl;
		return 1;
	}

	try {
		Server server( std::atoi( argv[1] ), argv[2] );
		globalServer = &server;

		std::signal( SIGINT, signalHandler );
		std::signal( SIGTERM, signalHandler );

		server.start();
	} catch ( const std::exception& e ) {
		std::cerr << RED << e.what() << std::endl;
	}

	return 0;

}
