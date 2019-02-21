#include <iostream>
#include <memory>

#include<stdio.h>	
#include<string.h>	
#include <unistd.h>	
#include<sys/socket.h>	
#include<arpa/inet.h>


// Connection settings
#include "settings.hpp"


// Main client thread
int main(int argc, const char* argv[]) {

	int		socketClient	= -1;		// Client socket

	sockaddr_in	server;				// Server address

	// Client message holder
	std::unique_ptr<char[]>	messageClient(new char[BUFFER_SIZE]);
	// Server message holder
	std::unique_ptr<char[]>	messageServer(new char[BUFFER_SIZE]);

	// Create socket
	socketClient = socket(AF_INET, SOCK_STREAM, 0);
	// check if socket was created
	if (socketClient == -1) {

		// Error during socket creation
		std::cout << "Could not create socket" << std::endl;

	}
	// Socket was created
	std::cout << "Socket created" << std::endl;

	// Zero the server address structure
	bzero(&server, sizeof(sockaddr_in));
	// Server port
	server.sin_family	= AF_INET;
	server.sin_addr.s_addr	= inet_addr(SERVER_ADDRESS);
	server.sin_port		= htons(SERVER_PORT);

	//Connect to remote server
	if (connect(socketClient, reinterpret_cast<sockaddr*>(&server), sizeof(server)) < 0) {

		std::cout << "Connect failed. Error" << std::endl;
		return 1;

	}
	// Connection to server - success
	std::cout << "Connected to server" << std::endl;
	
	// Run "forever"
	while (true) {

		// Message to send to server
		std::cout << "Enter message: ";
		std::cin >> messageClient.get();

		// Send some data to server
		if (write(socketClient, messageClient.get(), strlen(messageClient.get())) < 0) {

			std::cout << "Send failed" << std::endl;
			break;

		}

		// Cleanup buffer 
		memset(messageServer.get(), 0, BUFFER_SIZE);

		// Read reply from server
		if (read(socketClient, messageServer.get(), BUFFER_SIZE) < 0) {

			std::cout << "Recv failed" << std::endl;
			break;

		}

		// Output reply from server
		std::cout << "Server reply: " << messageServer.get() << std::endl;

	}

	// Close connection with server
	close(socketClient);

	// Exit client
	return 0;

}


