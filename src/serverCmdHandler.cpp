#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/ChannelCmds.hpp"

static void	handleCap( Server* server, int fd, const std::vector<std::string>& args ) {

	std::string reply;
	std::string subcmd = args[1];
	int argsNum = args.size();

	if ( subcmd == "END" ) {
		return;
	} else if ( subcmd == "LS" ) {
		reply = "CAP * LS :multi-prefix away-notify extended-join";
	} else if ( subcmd == "REQ" ) {
		reply = "CAP * ACK ";
		for ( int i = 2; i < argsNum; i++ ) {
			reply += args[i];
			if ( i != argsNum - 1 )
			reply += " ";
		}
	} else {
		server->sendError( fd, IRCerror::ERR_INVALIDCAPCMD, subcmd + ":Unknown CAP subcommand" );
		return;
	}

	server->sendToClient( fd, reply );

}

static void	handlePing( Server* server, int fd, const std::vector<std::string>& args ) {

	std::string reply = "PONG ";
	int argsNum = args.size();

	if ( args[1].find( ":" ) == std::string::npos )
		reply += ":";

	for ( int i = 1; i < argsNum; i++ ) {
		reply += args[i];
		if ( i != argsNum - 1 )
			reply += " ";
	}

	server->sendToClient( fd, reply );

}

static void	handlePass( Server* server, Client* client, const std::vector<std::string>& args ) {

	int fd = client->getSocketFd();

	// Check parameter exists
	if ( args.size() < 2 ) {
		server->sendError( fd, IRCerror::ERR_NEEDMOREPARAMS, "PASS :Not enough parameters" );
		return;
	}

	// Check for more parameters
	if ( args.size() > 2 ) {
		server->sendError( fd, IRCerror::ERR_PASSWDMISMATCH, "PASS :Too many parameters" );
		return;
	}

	// Check if already sent password
	if ( client->getIsAuthenticated() ) {
		server->sendError( fd, IRCerror::ERR_PASSWDMISMATCH, ":Password already attempted");
		return;
	}

	// Check if already registered
	if ( client->getIsResgistered() ) {
		server->sendError( fd, IRCerror::ERR_ALREADYREGISTRED, ":You may not register" );
		return;
	}

	std::string pass = args[1];
	if ( pass[0] == ':' )
		pass = pass.substr( 1 );

	if ( server->validatePassword( pass ) ) {
		client->setIsAuthenticated( true );
	} else {
		server->sendError( fd, IRCerror::ERR_PASSWDMISMATCH, ":Password incorrect" );
		server->disconnectClient( fd, "Bad password" );
	}

}

static void	handleQuit( Server* server, Client* client, const std::vector<std::string>& args ) {

	std::string reply = ":" + client->getNickname() +
						"!" + client->getUsername() +
						"@" + client->getHostname() +
						" QUIT ";

	if ( args[1].find( ":" ) == std::string::npos )
		reply += ":";

	int argsNum = args.size();
	for ( int i = 1; i < argsNum; i++ ) {
		reply += args[i];
		if ( i != argsNum - 1 )
			reply += " ";
	}

	server->broadcastMsgInClientChannels( client, reply );

	auto channels = server->getClientChannels( client );
	if ( !channels.empty() ) {
		for ( size_t i = 0; i < channels.size(); i++ )
			sendNameReplies( client, channels[i], *server );
	}

	server->disconnectClient( client->getSocketFd(), "Quit" );

}

static void	handleNotice( Server* server, std::string nickname, const std::vector<std::string>& args ) {

	for ( size_t i = 1; i < args.size(); i++ ) {
		if ( args[i].find( "LAGCHECK" ) != std::string::npos )
			server->sendToClient( nickname, "PONG :" + args[i + 1] );
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
	else if ( cmd == "PASS")
		handlePass( server, client, args );
	else if ( cmd == "QUIT")
		handleQuit( server, client, args );
	else if ( cmd == "NOTICE" )
		handleNotice( server, client->getNickname(), args );

}
