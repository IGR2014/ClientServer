#include <iostream>
#include <memory>
#include <cstring>

// Connection settings
#include "../settings.hpp"
// TCP client
#include <tcp/tcpclient.hpp>


// Main client thread
int main(int, const char*[]) {

	// Client message holder
	std::unique_ptr<char[]>	messageClient(new char[BUFFER_SIZE]);
	// Server message holder
	std::unique_ptr<char[]>	messageServer(new char[BUFFER_SIZE]);

	// Client creation
	networking::tcpclient client;

	// Connect to server
	if (client.connect(SERVER_ADDRESS, SERVER_PORT)) {

		// Connection to server - success
		std::cout << "Connected to server" << std::endl;

	}

	// Exchange data forever
	while (client.connected()) {

		// Message to send to server
		std::cout << "Enter message:\t";
		std::cin >> messageClient.get();

		// Send message to server
		client.send(messageClient.get(), strlen(messageClient.get()));

		// Cleanup buffer
		memset(messageServer.get(), 0, BUFFER_SIZE);

		// Receive message from server
		client.receive(messageServer.get(), BUFFER_SIZE);

		// Output reply from server
		std::cout << "Server reply:\t" << messageServer.get() << std::endl;

	}

	// Disconnect from server
	if (client.disconnect()) {

		// Disconnection from server - success
		std::cout << "Disconnected from server" << std::endl;

	}

	// Exit
	return 0;

}


