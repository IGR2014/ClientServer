#include <iostream>
#include <thread>
#include <memory>
#include <cstring>


// Connection settings
#include "settings.hpp"
// Socket type
#include "socket.hpp"
// TSP server
#include "tcp/tcpserver.hpp"


// Clients connection handler
void clientsConnectionHandler(networking::tcpserver &s, networking::socket_t* clientID) {

	// Connection handler init
	std::cout << "Connection handler started" << std::endl;

	// Client message holder
	std::unique_ptr<char[]>	messageClient(new char[BUFFER_SIZE]);

	// Read while data received
	while (s.receive(*clientID, messageClient.get(), BUFFER_SIZE)) {

		// Write message back to client
		if (!s.send(*clientID, messageClient.get(), strlen(messageClient.get()))) {

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
int main(int argc, const char* argv[]) {

	// Client message holder
	std::unique_ptr<char[]>	messageClient(new char[BUFFER_SIZE]);

	// Server creation
	networking::tcpserver s;

	// Start server
	if (s.start(SERVER_PORT)) {

		// Server startup - success
		std::cout << "Server started" << std::endl;

	}

	// Wait for connections forever
	while (s.running()) {

		// Wait for connection
		networking::socket_t* clientID = s.waitForConnect();

		// Create connection handler thread
		std::thread thrd = std::thread(clientsConnectionHandler, std::ref(s), clientID);
		// Run thread in background
		if (thrd.joinable()) {

			thrd.detach();

		}

	}

	// Stop server
	if (s.stop()) {

		// Server stoping - success
		std::cout << "Server stoped" << std::endl;

	}

	// Success
	return 0;

}


