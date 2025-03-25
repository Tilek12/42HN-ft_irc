#include <cassert>
#include <iostream>

#include "../include/Channel.hpp"
#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Command_handler.hpp"
#include "../include/ChannelCmds.hpp"

void test_channel_class()
{
    Channel channel("#TestChannel", "Initial Topic", false, false, "", 10);
    
    // Test getName
    assert(channel.getName() == "#TestChannel");
    
    // Test getTopic and setTopic
    assert(channel.getTopic() == "Initial Topic");
    channel.setTopic("New Topic");
    assert(channel.getTopic() == "New Topic");
    
    // Test adding and removing users
    channel.addUser("User1");
    channel.addUser("User2");
    assert(channel.getUsers().size() == 2);
    channel.removeUser("User1");
    assert(channel.getUsers().size() == 1);
    
    // Test adding and removing operators
    channel.addOperator("Oper1");
    assert(channel.getOperators().size() == 1);
    channel.removeOperator("Oper1");
    assert(channel.getOperators().size() == 0);
    
    // Test invite-only mode
    channel.setIsInviteOnly(true);
    assert(channel.getIsInviteOnly() == true);
    
    // Test password settings
    channel.setHasPassword(true);
    channel.setPassword("SecurePass");
    assert(channel.getHasPassword() == true);
    assert(channel.getPassword() == "SecurePass");
    
    // Test user limit
    channel.setUserLimit(20);
    assert(channel.getUserLimit() == 20);
}

void test_channel_commands()
{
    IClient* client = new Client(1, "host");
    IServer* server = new Server(6667, "123");
    
    // Test Join Channel without params
    std::vector<std::string> joinParams;
    ChannelCmds::joinChannelCmd(*client, *server, joinParams);
    
    // Test Part Channel without params
    std::vector<std::string> partParams;
    ChannelCmds::partChannelCmd(*client, *server, partParams);
    
    // Test Joining a new channel
    joinParams.push_back("#TestChannel");
    ChannelCmds:: joinChannelCmd(*client, *server, joinParams);
    
    // Test Leaving a channel
    partParams.push_back("#TestChannel");
    ChannelCmds::partChannelCmd(*client, *server, partParams);
}

int main()
{
    test_channel_class();
    test_channel_commands();
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
