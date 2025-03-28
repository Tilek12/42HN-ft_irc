// #include <iostream>
// #include "../include/Client.hpp"
// #include "../include/client_c_h.hpp"
// #include "../include/Server.hpp"

// int main() {
//     Server *server = new Server(6667, "123");
//     CommandHandler *handler = new CommandHandler(*server);

//     // Creating test clients with a file descriptor and hostname
//     Client client1(1, "host1.local");
//     Client client2(2, "host2.local");
//     Client client3(3, "host3.local");
//     Client client4(3, "host3.local");


//     // Simulating commands
//     handler->handleCommand(&client1, "NICK Rustam");
//     handler->handleCommand(&client2, "NICK Tilek");
//     handler->handleCommand(&client3, "NICK Linda");
//     handler->handleCommand(&client4, "USER testuser 0 * :Test Real Name");

//     server->addClient(&client1);
//     server->addClient(&client2);
//     server->addClient(&client3);
//     server->addClient(&client4);

//     handler->handleCommand(&client1, "PRIVMSG Linda :Hello!");
//     delete server;
//     delete handler;

//     return 0;
// }
