
#include "../include/Server.hpp"

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

	int port = std::atoi( argv[1] );
	const std::string password = argv[2];

	try {
		Server* server = new Server( port, password );
		server->start();
	} catch ( const std::exception& e ) {
		std::cerr << RED << e.what() << std::endl;
	}

	return 0;

}
