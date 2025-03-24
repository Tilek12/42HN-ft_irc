#ifndef ISERVER_HPP
#define ISERVER_HPP

#include <iostream>

#include "IChannel.hpp"

class IServer
{
public:
	virtual ~IServer();

	virtual IChannel* getChannel(const std::string& channelName) = 0;
	virtual IChannel* createChannel(const std::string& channelName) = 0;
	virtual IChannel* removeChannel(const std::string& channelName) = 0;
};

#endif