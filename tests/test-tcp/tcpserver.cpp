#include <iostream>
#include <thread>
#include <memory>
#include <cstring>


// Connection settings
#include "../settings.hpp"
// Socket type
#include <tcp/tcpsocket.hpp>
// TCP server
#include <tcp/tcpserver.hpp>


// Clients connection handler
void clientsConnectionHandler(networking::tcpserver &server, networking::tcpsocket_t* clientID) {

	// Connection handler init
	std::cout << "Client connected" << std::endl;

	// Client message holder
	std::unique_ptr<char[]>	messageClient(new char[BUFFER_SIZE]);

	// Read while data received
	while (server.receive(*clientID, messageClient.get(), BUFFER_SIZE)) {

		// Write message back to client
		if (!server.send(*clientID, messageClient.get(), strlen(messageClient.get()))) {

			// No client
			std::cout << "Client diconnected" << std::endl;

			// Stop
			return;

		} else {

			// Message from client
			std::cout << "Got message:\t" << strlen(messageClient.get()) << "\t" << messageClient.get() << std::endl;

		}

		// Cleanup receive buffer
		memset(messageClient.get(), 0, BUFFER_SIZE);

	}

	// Cleanup
	delete clientID;

	// No client
	std::cout << "Client diconnected" << std::endl;

}


// Main server thread
int main(int, const char*[]) {

	// Client message holder
	std::unique_ptr<char[]>	messageClient(new char[BUFFER_SIZE]);

	// Server creation
	networking::tcpserver server;

	// Start server
	if (server.start(SERVER_PORT)) {

		// Server startup - success
		std::cout << "Server started" << std::endl;

	}

	// Wait for connections forever
	while (server.running()) {

		// Wait for connection
		networking::tcpsocket_t* clientID = server.waitForConnect();

		// Create connection handler thread
		std::thread thrd = std::thread(clientsConnectionHandler, std::ref(server), clientID);
		// Run thread in background
		if (thrd.joinable()) {

			thrd.detach();

		}

	}

	// Stop server
	if (server.stop()) {

		// Server stoping - success
		std::cout << "Server stoped" << std::endl;

	}

	// Success
	return 0;

}


