#include <cassert>
#include <iostream>

#include "../include/Channel.hpp"

void test_channel()
{
    Channel ch("#TestChannel", "Initial Topic", false, false, "", 10);
    
    // Test getName
    assert(ch.getName() == "#TestChannel");
    
    // Test getTopic and setTopic
    assert(ch.getTopic() == "Initial Topic");
    ch.setTopic("New Topic");
    assert(ch.getTopic() == "New Topic");
    
    // Test adding and removing users
    ch.addUser("User1");
    ch.addUser("User2");
    assert(ch.getUsers().size() == 2);
    ch.removeUser("User1");
    assert(ch.getUsers().size() == 1);
    
    // Test adding and removing operators
    ch.addOperator("Oper1");
    assert(ch.getOperators().size() == 1);
    ch.removeOperator("Oper1");
    assert(ch.getOperators().size() == 0);
    
    // Test invite-only mode
    ch.setIsInviteOnly(true);
    assert(ch.getIsInviteOnly() == true);
    
    // Test password settings
    ch.setHasPassword(true);
    ch.setPassword("SecurePass");
    assert(ch.getHasPassword() == true);
    assert(ch.getPassword() == "SecurePass");
    
    // Test user limit
    ch.setUserLimit(20);
    assert(ch.getUserLimit() == 20);
    
    std::cout << "All tests passed successfully!" << std::endl;
}

int main()
{
    test_channel();
    return 0;
}
