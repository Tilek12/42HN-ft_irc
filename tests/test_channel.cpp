#include <cassert>
#include <iostream>

#include "../include/Channel.hpp"
#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/client_c_h.hpp"
#include "../include/ChannelCmds.hpp"
#include "../include/client_c_h.hpp"
#include "../include/channelHelperFcts.hpp"

void test_channel_class()
{
    Channel channel("#TestChannel");

    // Test getName
    assert(channel.getName() == "#TestChannel");

    // Test getTopic and setTopic
    assert(channel.getTopic() == "");
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
    Server* server = new Server(6667, "123");
    Client client1(1, "host1");
    Client client2(2, "host2");
    Client client3(3, "host3");
    Client client4(4, "host4");

    CommandHandler *handler = new CommandHandler(*server);
    handler->handleCommand(&client1, "NICK user1");
    handler->handleCommand(&client2, "NICK user2");
    handler->handleCommand(&client3, "NICK user3");
    handler->handleCommand(&client4, "NICK user4");

    // Test Join Channel without params => error expected
    std::vector<std::string> joinParams;
    ChannelCmds::joinChannelCmd(client1, *server, joinParams);

    // Test Part Channel without params => error expected
    std::vector<std::string> partParams;
    ChannelCmds::partChannelCmd(client1, *server, partParams);

    // Test Kicking a user without params => error expected
    std::vector<std::string> kickParams;
    ChannelCmds::kickUserCmd(client1, *server, kickParams);

     // Test Invite without parameters => error expected
     std::vector<std::string> inviteParams;
     ChannelCmds::inviteUserCmd(client1, *server, inviteParams);

    // Test Joining multiple channels
    joinParams.push_back("#Channel1");
    ChannelCmds::joinChannelCmd(client1, *server, joinParams);
    ChannelCmds::joinChannelCmd(client2, *server, joinParams);
    ChannelCmds::joinChannelCmd(client3, *server, joinParams);

    joinParams[0] = "#Channel2";
    ChannelCmds::joinChannelCmd(client1, *server, joinParams);
    ChannelCmds::joinChannelCmd(client3, *server, joinParams);

    // Test Leaving a channel
    partParams.push_back("#Channel1");
    ChannelCmds::partChannelCmd(client2, *server, partParams);

    // Test Kicking a user with a reason
    kickParams.push_back("#Channel2");
    kickParams.push_back("user3");
    kickParams.push_back("Spamming the chat");
    ChannelCmds::kickUserCmd(client1, *server, kickParams);

    // Test Kicking a user from a single channel
    joinParams[0] = "#Channel2";
    ChannelCmds::joinChannelCmd(client3, *server, joinParams);
    kickParams[0] = "#Channel1,#Channel2";
    kickParams[1] = "user99,user3";
    ChannelCmds::kickUserCmd(client1, *server, kickParams);

    // Test inviting a user to a not isInvitedOnly channel => error
    inviteParams.push_back("user4");
    inviteParams.push_back("#Channel1");
    ChannelCmds::inviteUserCmd(client1, *server, inviteParams);

     // Test inviting a user to a channel
    Channel* channel = server->getChannel("#Channel1");
    channel->setIsInviteOnly(true);
    inviteParams.push_back("user4");
    inviteParams.push_back("#Channel1");
    ChannelCmds::inviteUserCmd(client1, *server, inviteParams);

    // Test inviting a non-existent user => error
    inviteParams.clear(); 
    inviteParams.push_back("user42");
    inviteParams.push_back("#Channel1");
    ChannelCmds::inviteUserCmd(client1, *server, inviteParams);

    // // Test inviting a user already in the channel => error expected
    // inviteParams[0] = "user3";
    // ChannelCmds::inviteUserCmd(client1, *server, inviteParams);
 
}

int main()
{
    test_channel_class();
    test_channel_commands();
    return 0;
}
