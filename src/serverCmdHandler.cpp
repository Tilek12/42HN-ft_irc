#include "../include/Server.hpp"
#include "../include/Client.hpp"

static void	handleCap( Server* server, int fd, const std::vector<std::string>& args ) {

	std::string reply;

	if ( args[1] == "END" )
		return;
	if ( args[1] == "LS" )
		reply = "CAP * LS :multi-prefix away-notify extended-join\r\n";
	else if ( args[1] == "REQ" ) {
		reply = "CAP * ACK ";
		for ( int i = 2; i < args.size(); i++ )
			reply += args[i];
		reply += "\r\n";
	}

	server->sendToClient( fd, reply );

}

static void	handlePing( Server* server, int fd, const std::vector<std::string>& args ) {

	std::string reply = "PONG ";

	if ( args[1].find( ":" ) != std::string::npos )
		reply += ":";

	for ( int i = 1; i < args.size() - 1; i++ ) {
		reply += args[i];
		if ( i != args.size() - 1 )
			reply += " ";
	}
	reply += "\r\n";

	server->sendToClient( fd, reply );

}

static void	handleNotice( Server* server, std::string nickname, const std::vector<std::string>& args ) {

	for ( int i = 1; i < args.size() - 1; i++ ) {
		if ( args[i].find( "LAGCHECK" ) != std::string::npos ) {
			// std::string reply = "NOTICE " + nickname + " :LAGCHECK_REPLY " +
			// 					args[i].substr( args[i].find( "LAGCHECK" ) + 9 ) + "\r\n";
			std::string reply = "PONG :" + args[i + 1] + "\r\n";
			server->sendToClient( nickname, reply );
		}
	}

}

void	serverCmdHandler( Server* server, Client* client ) {

	const auto& args = server->getArguments();
	if ( args.empty() )
		return;

	std::string cmd = args[0];

	if ( cmd == "CAP" )
		handleCap( server, client->getSocketFd(), args );
	else if ( cmd == "PING" )
		handlePing( server, client->getSocketFd(), args );
	else if ( cmd == "NOTICE" )
		handleNotice( server, client->getNickname(), args );

}
