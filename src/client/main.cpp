#include <iostream>
#include "client.hpp"
#include "client_c_h.hpp"


int main() {
    Server server;
    CommandHandler handler(server);

    // Creating test clients with a file descriptor and hostname
    Client client1(1, "host1.local");
    Client client2(2, "host2.local");
    Client client3(3, "host3.local");
    Client client4(3, "host3.local");


    // Simulating commands
    handler.Handle_client_command(&client1, "NICK Rustam");
    handler.Handle_client_command(&client2, "NICK Tilek");
    handler.Handle_client_command(&client3, "NICK Linda");
    handler.Handle_client_command(&client4, "USER testuser 0 * :Test Real Name");

    server.addClient(&client1);
    server.addClient(&client2);
    server.addClient(&client3);
    server.addClient(&client4);

    handler.Handle_client_command(&client1, "PRIVMSG Linda :Hello!");

    return 0;
}
