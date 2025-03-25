
#pragma once

#include <string>

#include "Client.hpp"
#include "Channel.hpp"

class	IServer {

public:

	// Core server operations
	virtual						~IServer( void ) {}
	virtual void				start( void ) = 0;
	virtual void				stop( void ) = 0;
	virtual const std::string&	getPassword( void ) const = 0;

	// Client management
	virtual Client*		getClient( int fd ) const = 0;
	virtual Client*		getClient( const std::string& nickname ) const = 0;

	// Channel management
	virtual Channel*	getChannel( const std::string& name ) const = 0 ;
	virtual Channel*	createChannel( const std::string& name ) = 0;
	virtual void		removeChannel( const std::string& name ) = 0;

	// Messaging
	virtual void	sendToClient( int fd, const std::string& message ) = 0;
	// virtual void	broadcastMessage( const std::string& channel, const std::string& message ) = 0;

};
