
#pragma once

#include <string>

#include "Client.hpp"
#include "Channel.hpp"

class	IServer {

public:

	// Core server operations
	virtual								~IServer( void ) {}
	virtual void						start( void ) = 0;
	virtual void						stop( void ) = 0;
	virtual bool						validatePassword( const std::string& pass ) const = 0;
	virtual bool						getIsRunning( void ) = 0;
	virtual void						setIsRunning( bool value ) = 0;
	virtual std::vector<std::string>	getArguments( void ) = 0;

	// Client management
	virtual void		addClient( Client* client ) = 0;
	virtual Client*		getClient( int fd ) const = 0;
	virtual Client*		getClient( const std::string& nickname ) const = 0;
	virtual bool		isClientExist( const std::string& nickname ) = 0;
	virtual bool		isClientExist( int fd ) = 0;
	virtual bool		checkClientAuthentication( Client* client ) = 0;
	virtual void		disconnectClient( int fd, const std::string& reason ) = 0;
	virtual void		removeClientFromChannels( Client* client ) = 0;

	// Channel management
	virtual void							addChannel( Channel* channel ) = 0;
	virtual Channel*						getChannel( const std::string& name ) const = 0;
	virtual std::map<std::string, Channel*>	getChannels( void ) = 0;
	virtual Channel*						createChannel( const std::string& name ) = 0;
	virtual void							removeChannel( const std::string& name ) = 0;
	virtual bool							isChannelExist( const std::string& name ) = 0;
	virtual std::vector<Channel*>			getClientChannels( Client* client ) = 0;

	// Messaging
	virtual void	sendToClient( int fd, const std::string& message ) = 0;
	virtual void	sendToClient( const std::string& nickname, const std::string& message ) = 0;
	virtual void	sendError( int fd, const std::string& errorCode, const std::string& message ) = 0;
	virtual void	broadcastMessage( Client* client,
									  const std::string& channelName,
									  const std::string& message ) = 0;
	virtual void	broadcastMsgInClientChannels( Client* client, const std::string& message ) = 0;

};
