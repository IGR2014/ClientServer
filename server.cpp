#include <iostream>
#include <thread>
#include <memory>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>


// Connection settings
#include "settings.hpp"


// Clients connection handler
void* connection_handler(int* socket_desc) {

	// Socket descriptor
	int sock	= *socket_desc;
	// Number of readed bytes
	int readed	= 0;

	// Client message holder
	std::unique_ptr<char[]>	messageClient(new char[BUFFER_SIZE]);

	// Read a message from client
	while ((readed = read(sock, messageClient.get(), BUFFER_SIZE)) > 0) {

		// Output what was readed
		std::cout << "Readed:\t" << readed << "\t" << messageClient.get() << std::endl;

		// Send the message back to client
		if (write(sock, messageClient.get(), readed) < 0) {

			std::cout << "Write failed" << std::endl;

		}

		// Cleanup buffer
		memset(messageClient.get(), 0, BUFFER_SIZE);

	}

	// If 0 bytes readed - client was disconnected
	if (readed == 0) {

		std::cout << "Client disconnected" << std::endl;

	// If -1 was returned - some errors during read
	} else if(readed == -1) {

		std::cout << "Read failed" << std::endl;

	}
		
	// Free the socket pointer
	delete socket_desc;

	// Exit successfully
	return 0;

}


// Main server thread
int main(int argc, const char* argv[]) {

	int		socketServer	= -1;		// Server socket
	int		socketClient	= -1;		// Client socket

	sockaddr_in	server;				// Server address
	sockaddr_in	client;				// Client address

	// Create TCP socket
	socketServer = socket(AF_INET, SOCK_STREAM, 0);
	// Socket creation error
	if (socketServer == -1) {

		std::cout << "Could not create socket" << std::endl;

	}
	// Socket creation succeeded
	std::cout << "Socket created" << std::endl;

	// Multiclient flag
	int multiclient = 1;
	// Enable multiclient connect
	if (setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &multiclient, sizeof(int)) < 0) {

		std::cout << "Could not allow SO_REUSEADDR | SO_REUSEPORT" << std::endl;

	}
	//
	std::cout << "Socket allowed SO_REUSEADDR | SO_REUSEPORT" << std::endl;

	// Zero the server address structure
	bzero(&server, sizeof(sockaddr_in));
	// Prepare the server address structure
	server.sin_family	= AF_INET;
	server.sin_addr.s_addr	= INADDR_ANY;
	server.sin_port		= htons(SERVER_PORT);

	// Bind socket to port
	if (bind(socketServer, reinterpret_cast<sockaddr*>(&server), sizeof(server)) < 0) {

		// Error during binding
		std::cout << "Bind failed. Error" << std::endl;
		return 1;

	}
	// Binding to port success
	std::cout << "Bind done" << std::endl;
	
	// Listen for port (maximum 10 connections)
	listen(socketServer, 10);

	//Accept and incoming connection
	std::cout << "Waiting for incoming connections..." << std::endl;
	socklen_t clientLength = sizeof(sockaddr_in);

	// Wait for connection
	while ((socketClient = accept(socketServer, reinterpret_cast<sockaddr*>(&client), static_cast<socklen_t*>(&clientLength))) > 0) {

		// Connection was accepted
		std::cout << "Connection accepted" << std::endl;

		// Create connection handler thread
		std::thread thrd = std::thread(connection_handler, new int(socketClient));
		// Run thread in background
		if (thrd.joinable()) {

			thrd.detach();

		}

		// Handler was successfully assigned
		std::cout << "Handler assigned" << std::endl;

	}

	// Connection accept was failed
	if (socketClient < 0) {

		std::cout << "Accept failed" << std::endl;
		return 1;

	}

	// Exit program
	return 0;

}


