
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main ( void ) {
	// Creating socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	// Setting the address
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Binding socket
	bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));

	// Listening to the assigned socket
	listen(serverSocket, 5);

	// Accepting connection request
	int clientSocket = accept(serverSocket, nullptr, nullptr);

	// Recieving data
	char buffer[1024] = { 0 };
	recv(clientSocket, buffer, sizeof(buffer), 0);
	std::cout << "Message from client: " << buffer << std::endl;

	// Close the socket
	close(serverSocket);

	return 0;
}

