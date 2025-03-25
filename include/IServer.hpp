
#pragma once

#include <string>

class	IServer {

public:

	virtual				~IServer( void ) {}
	virtual void		start( void ) = 0;
	virtual void		stop( void ) = 0;
	virtual std::string	getPassword( void ) = 0;

};
