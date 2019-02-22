#include <iostream>
#include <memory>
#include <cstring>

// Connection settings
#include "settings.hpp"
// TCP client
#include "tcp/tcpclient.hpp"


// Main client thread
int main(int argc, const char* argv[]) {

	// Client message holder
	std::unique_ptr<char[]>	messageClient(new char[BUFFER_SIZE]);
	// Server message holder
	std::unique_ptr<char[]>	messageServer(new char[BUFFER_SIZE]);

	// Client creation
	networking::tcpclient c;

	// Connect to server
	if (c.connect(SERVER_ADDRESS, SERVER_PORT)) {

		// Connection to server - success
		std::cout << "Connected to server" << std::endl;

	}

	// Exchange data forever
	while (c.connected()) {

		// Message to send to server
		std::cout << "Enter message:\t";
		std::cin >> messageClient.get();

		// Send message to server
		c.send(messageClient.get(), strlen(messageClient.get()));

		// Cleanup buffer
		memset(messageServer.get(), 0, BUFFER_SIZE);

		// Receive message from server
		c.receive(messageServer.get(), BUFFER_SIZE);

		// Output reply from server
		std::cout << "Server reply:\t" << messageServer.get() << std::endl;

	}

	// Disconnect from server
	if (c.disconnect()) {

		// Disconnection from server - success
		std::cout << "Disconnected from server" << std::endl;

	}

	// Exit
	return 0;

}


