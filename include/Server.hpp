
#pragma once

#include <string>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include "colors.hpp"

/*-------------------*/
/*  Define IRC port  */
/*-------------------*/
const int IRCport = 6667;

/*-----------------------*/
/*  Define Server class  */
/*-----------------------*/
class Server {

private:

	int			_serverSocket;
	int			_clientSocket;
	std::string	_password;

	void	_setupServer( void );
	void	_handleConnection( void );
	void	_handleData( void );

public:

	Server( int port, const std::string& password );
	~Server( void );
	Server( const Server& other ) = delete;
	Server& operator=( const Server&  other ) = delete;

	void	run( void );

};
