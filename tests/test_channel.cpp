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
    
    // Test Topic without parameters => error expected
    std::vector<std::string> topicParams;
    ChannelCmds::topicUserCmd(client1, *server, inviteParams);

    // Test Joining multiple channels
    joinParams.push_back("#Channel1");
    ChannelCmds::joinChannelCmd(client1, *server, joinParams);
    ChannelCmds::joinChannelCmd(client2, *server, joinParams);
    ChannelCmds::joinChannelCmd(client3, *server, joinParams);
    assert(server->getChannel("#Channel1")->getUsers().size() == 3);

    joinParams.clear();
    joinParams.push_back("#Channel2");
    ChannelCmds::joinChannelCmd(client1, *server, joinParams);
    ChannelCmds::joinChannelCmd(client3, *server, joinParams);

    // Test Leaving a channel
    partParams.push_back("#Channel1");
    ChannelCmds::partChannelCmd(client2, *server, partParams);
    assert(server->getChannel("#Channel1")->getUsers().size() == 2);

    // Test Kicking a user with a reason
    kickParams.push_back("#Channel2");
    kickParams.push_back("user3");
    kickParams.push_back("Spamming the chat");
    ChannelCmds::kickUserCmd(client1, *server, kickParams);
    assert(server->getChannel("#Channel2")->getUsers().size() == 1);

    // Test Kicking a user from a multiple channels
    joinParams.clear();
    joinParams.push_back("#Channel2");
    ChannelCmds::joinChannelCmd(client3, *server, joinParams);
    kickParams.clear();
    kickParams.push_back("#Channel1,#Channel2");
    kickParams.push_back("user99,user3");
    ChannelCmds::kickUserCmd(client1, *server, kickParams);
    assert(server->getChannel("#Channel2")->getUsers().size() == 1);

    // Test inviting a user to a not isInvitedOnly channel => error
    inviteParams.push_back("user4");
    inviteParams.push_back("#Channel1");
    ChannelCmds::inviteUserCmd(client1, *server, inviteParams);
    assert(server->getChannel("#Channel1")->getInvitedUsers().size() == 0);

    // Test inviting a user to a channel
    Channel* channel = server->getChannel("#Channel1");
    channel->setIsInviteOnly(true);
    inviteParams.push_back("user4");
    inviteParams.push_back("#Channel1");
    ChannelCmds::inviteUserCmd(client1, *server, inviteParams);
    assert(server->getChannel("#Channel1")->getInvitedUsers().size() == 1);

    // Test inviting a user already in _invitedUsers
    ChannelCmds::inviteUserCmd(client1, *server, inviteParams);
    assert(server->getChannel("#Channel1")->getInvitedUsers().size() == 1);

    // Test user3 is no operator, so he can not invite a user to a channel => error
    ChannelCmds::inviteUserCmd(client3, *server, inviteParams);
    assert(server->getChannel("#Channel1")->getInvitedUsers().size() == 1);

    // Test inviting a user
    inviteParams.clear(); 
    inviteParams.push_back("user42");
    inviteParams.push_back("#Channel1");
    ChannelCmds::inviteUserCmd(client1, *server, inviteParams);
    assert(server->getChannel("#Channel1")->getInvitedUsers().size() == 2);

    // Test changing topic
    topicParams.push_back("#Channel1");
    topicParams.push_back(":my changed topic");
    // error expected, only operator can changed topic
    ChannelCmds::topicUserCmd(client3, *server, topicParams);
    // change topic
    ChannelCmds::topicUserCmd(client1, *server, topicParams); 
    assert(channel->getTopic() == "my changed topic");
    
    // Test get topic
    topicParams.clear();
    topicParams.push_back("#Channel1");
    ChannelCmds::topicUserCmd(client1, *server, topicParams);
    assert(channel->getTopic() == "my changed topic");

    // Test clear topic
    topicParams.clear();
    topicParams.push_back("#Channel1");
    topicParams.push_back(":");
    ChannelCmds::topicUserCmd(client1, *server, topicParams); 
    assert(channel->getTopic() == "");

    server->stop();
}

void test_channel_modes()
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

    // Test  without parameters => error expected
    std::vector<std::string> modeParams;
    ChannelCmds::modeUserCmd(client1, *server, modeParams);

    // Test: Create a channel and make client1 the operator
    std::vector<std::string> joinParams = {"#TestChannel"};
    ChannelCmds::joinChannelCmd(client1, *server, joinParams);
    ChannelCmds::joinChannelCmd(client2, *server, joinParams);
    ChannelCmds::joinChannelCmd(client3, *server, joinParams);

    // Test: +i (Invite-Only Mode)
    modeParams.push_back("#TestChannel");
    modeParams.push_back("+i");
    ChannelCmds::modeUserCmd(client1, *server, modeParams);
    assert(server->getChannel("#TestChannel")->getIsInviteOnly() == true);
    
    // Test: -i (Remove Invite-Only Mode)
    modeParams.clear();
    modeParams.push_back("#TestChannel");
    modeParams.push_back("-i");
    ChannelCmds::modeUserCmd(client2, *server, modeParams); // error expected
    ChannelCmds::modeUserCmd(client1, *server, modeParams);
    assert(server->getChannel("#TestChannel")->getIsInviteOnly() == false);
    
    // Test: -t (Remove Only Operator Can Change Topic Mode)
    modeParams.clear();
    modeParams.push_back("#TestChannel");
    modeParams.push_back("-t");
    ChannelCmds::modeUserCmd(client1, *server, modeParams);
    assert(server->getChannel("#TestChannel")->getOnlyOperatorCanChangeTopic() == false);
    
    std::vector<std::string> topicParams;
    topicParams.push_back("#TestChannel");
    topicParams.push_back(":hello new topic here from non operator user3");
    ChannelCmds::topicUserCmd(client3, *server, topicParams);
    assert(server->getChannel("#TestChannel")->getTopic() == "hello new topic here from non operator user3");

    // Test: +t (Only Operator Can Change Topic)
    modeParams.clear();
    modeParams.push_back("#TestChannel");
    modeParams.push_back("+t");
    topicParams.clear();
    topicParams.push_back("#TestChannel");
    topicParams.push_back(":hello new topic here");
    ChannelCmds::modeUserCmd(client1, *server, modeParams);
    ChannelCmds::topicUserCmd(client1, *server, topicParams);
    assert(server->getChannel("#TestChannel")->getOnlyOperatorCanChangeTopic() == true);
    assert(server->getChannel("#TestChannel")->getTopic() == "hello new topic here");

    // Test: +l (Set User Limit to 10)
    modeParams.clear();
    modeParams.push_back("#TestChannel");
    modeParams.push_back("+l");
    modeParams.push_back("3");
    ChannelCmds::modeUserCmd(client1, *server, modeParams);
    assert(server->getChannel("#TestChannel")->getUserLimit() == 3);
    ChannelCmds::joinChannelCmd(client3, *server, joinParams);
    assert(server->getChannel("#TestChannel")->getUserLimit() == 3);


    // Test: -l (Remove User Limit)
    modeParams = {"#TestChannel", "-l"};
    ChannelCmds::modeUserCmd(client1, *server, modeParams);
    assert(server->getChannel("#TestChannel")->getUserLimit() == 0);

    // Test: +k (Set Channel Password)
    modeParams.clear();
    modeParams.push_back("#TestChannel");
    modeParams.push_back("+k");
    modeParams.push_back("secretpass");
    ChannelCmds::modeUserCmd(client1, *server, modeParams);
    assert(server->getChannel("#TestChannel")->getHasPassword() == true);
    assert(server->getChannel("#TestChannel")->getPassword() == "secretpass");

    // Test: -k (Remove Channel Password)
    modeParams.clear();
    modeParams.push_back("#TestChannel");
    modeParams.push_back("-k");
    modeParams.push_back("secretpass");
    ChannelCmds::modeUserCmd(client1, *server, modeParams);
    assert(server->getChannel("#TestChannel")->getHasPassword() == false);
    assert(server->getChannel("#TestChannel")->getPassword() == "");

    // Test: Invalid Mode
    modeParams.clear();
    modeParams.push_back("#TestChannel");
    modeParams.push_back("-z");
    ChannelCmds::modeUserCmd(client1, *server, modeParams);

    server->stop();
}

int main()
{
    test_channel_class();
    test_channel_commands();
    test_channel_modes();
    return 0;
}
