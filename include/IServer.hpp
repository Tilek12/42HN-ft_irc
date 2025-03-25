
#pragma once

#include <string>

class	IServer {

public:

	// Core server operations
	virtual						~IServer( void ) {}
	virtual void				start( void ) = 0;
	virtual void				stop( void ) = 0;
	virtual const std::string&	getPassword( void ) const = 0;

	// Client management
	virtual Client*				getClient( int fd ) const = 0;
	virtual Client*				getClient( const std::string& nickname ) const = 0;

	// Channel management
	virtual Channel*			getChannel( const std::string& name ) const = 0 ;
	virtual Channel*			createChannel( const std::string& name, Client* creator, const std::string& key = "" ) = 0;
	void						removeChannel( const std::string& name ) = 0;

};
