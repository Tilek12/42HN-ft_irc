
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../include/Client.hpp"
#include "../include/Server.hpp"

// int main()
// {
// 	Client* client = new Client(1, "host");
//     Server* server = new Server(6667, "123");
// 	// creating socket
// 	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
// 	// specifying address
// 	sockaddr_in serverAddress;
// 	serverAddress.sin_family = AF_INET;
// 	serverAddress.sin_port = htons(6667);
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;

// 	// sending connection request
// 	connect(clientSocket, (struct sockaddr*)&serverAddress,
// 	sizeof(serverAddress));

// 	// sending data
// 	const char* message = "Hello, server!";
// 	// send(clientSocket, message, strlen(message), 0);
// 	ssize_t result = send(clientSocket, message, strlen(message), 0);

// 	if ( result < 0 )
// 		std::cout << "--- send didn't work ---" << std::endl;

// 	// closing socket
// 	close(clientSocket);

// 	return 0;
// }

int main()
{
	Client* client = new Client(1, "host");
    Server* server = new Server(6667, "123");

    // creating socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("socket failed");
        return 1;
    }

    // specifying address
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(6667);

    // Convert IP to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        perror("Invalid address");
        close(clientSocket);
        return 1;
    }

    // sending connection request
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("connect failed");
        close(clientSocket);
        return 1;
    }

    // sending data
    const char* message = "Hello, server!";
    ssize_t result = send(clientSocket, message, strlen(message), 0);

    if (result == -1) {
        perror("send failed");
    } else {
        std::cout << "Sent " << result << " bytes to the server.\n";
    }

    // closing socket
    close(clientSocket);

    return 0;
}
