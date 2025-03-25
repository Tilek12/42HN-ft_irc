
#pragma once

#include <string>

class	IServer {

public:

	virtual						~IServer( void ) {}
	virtual void				start( void ) = 0;
	virtual void				stop( void ) = 0;
	virtual const std::string&	getPassword( void ) const = 0;

};
