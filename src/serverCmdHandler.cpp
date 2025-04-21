#include "../include/Server.hpp"
#include "../include/Client.hpp"

static void	handleCap( Server* server, int fd, const std::vector<std::string>& args ) {

	std::string reply;
	std::string subcmd = args[1];
	int argsNum = args.size();

	if ( subcmd == "END" )
		return;
	else if ( subcmd == "LS" )
		reply = "CAP * LS :multi-prefix away-notify extended-join";
	else if ( subcmd == "REQ" ) {
		reply = "CAP * ACK ";
		for ( int i = 2; i < argsNum; i++ ) {
			reply += args[i];
			if ( i != argsNum - 1 )
			reply += " ";
		}
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

	// Check if already registered
	if ( client->getIsResgistered() ) {
		server->sendError( fd, IRCerror::ERR_ALREADYREGISTRED, ":You may not register" );
		return;
	}

	// Check if already sent password
	if ( client->getIsAuthenticated() ) {
		server->sendError( fd, IRCerror::ERR_PASSWDMISMATCH, ":Password already attempted");
		return;
	}

	// Check parameter exists
	if ( args.size() < 2 ) {
		server->sendError( fd, IRCerror::ERR_NEEDMOREPARAMS, "PASS :Not enough parameters" );
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
		for ( int i = 0; i < channels.size(); i++ )
		{
			// server->sendNamesList( channels[i] );
			std::string userList;
			for (auto &&i : channels[i]->getUsers())
				userList += i + " ";
			std::string reply353 = ":" + IRCname + " " + IRCreply::RPL_NAMREPLY + " " +
								   client->getNickname() + " = " + channels[i]->getName() + " :" + userList;
			server->sendToClient(client->getNickname(), reply353);
			std::string reply366 = ":" + IRCname + " " + IRCreply::RPL_ENDOFNAMES + " " +
								   client->getNickname() + " " + channels[i]->getName() + " :End of /NAMES list";
			server->sendToClient(client->getNickname(), reply366);

		}
	}

	server->disconnectClient( client->getSocketFd(), "Quit" );

}

static void	handleNotice( Server* server, std::string nickname, const std::vector<std::string>& args ) {

	for ( int i = 1; i < args.size(); i++ ) {
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
