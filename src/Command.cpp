#include <vector>
#include <iostream>

#include "../include/errorReplies.hpp"
#include "../include/Channel.hpp"
#include "../include/Command.hpp"

#include "../include/IClient.hpp"
#include "../include/IServer.hpp"

static void joinChannelCmd(IClient& client, IServer& server, std::vector<std::string>& joinParams)
{	
	Channel* channel;
	std::string channelName;
	std::string password;
	vector<std::string> passwords;

	if (joinParams.empty())
	{
		std::cerr << "Error code " << ERR_NEEDMOREPARAMS << \
			" JOIN: not enough joinParams" << std::endl;
		return ;
	}

	
	std::stringstream ssChannels(joinParams[0]);
	std::stringstream ssPasswords(joinParams[1]);
	if (joinParams.size() > 1)
	{
		while (getline(ssPasswords, password, ','))
		{
			passwords.push_back(password)
		}
	}

	while (getline(ssChannels, channelName, ','))
	{
		if (isValidChannelName(channelName))
			channel = server.getChannel(channelName;)
			if (channel._hasPassword && channel._password == password)
			{
				/* code */
			}
			
		if (!channel)
			channel = server.createChannel(channelName);
		
	}

	

}
