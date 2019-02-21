#include <iostream>
#include <memory>
	
#include <cstring>
#include <unistd.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>


// Connection settings
#include "settings.hpp"


// Networking namespace
namespace networking {


	// TCP Client class
	class clienttcp {

		sockaddr_in	server;			// Server address

		int		socketClient;		// Client socket

		bool		isConnected;		// Is connected to server


	public:

		// C-tor
		clienttcp();
		// C-tor
		clienttcp(const char* serverAddress, const unsigned int serverPort);
		// D-tor
		~clienttcp();

		// Connect to server
		bool	connect(const char* serverAddress, const unsigned int serverPort);
		// Disconnect from server
		bool	disconnect();

		// Send message to server
		bool	send(const char* msg, const int size);
		// Receive message from server
		bool	receive(char* msg, const int size);

		// Is client connected to server
		bool	connected();


	};


	// C-tor
	clienttcp::clienttcp()
		: server	(),
		  socketClient	(-1),
		  isConnected	(false) {}

	// C-tor
	clienttcp::clienttcp(const char* serverAddress, const unsigned int serverPort)
		: server	(),
		  socketClient	(-1),
		  isConnected	(false) {

		// Connect to server
		connect(serverAddress, serverPort);

	}

	// D-tor
	clienttcp::~clienttcp() {

		// Close connection if connected
		if (isConnected) {

			disconnect();

		}

	}


	// Connect to server
	bool clienttcp::connect(const char* serverAddress, const unsigned int serverPort) {

		// Create socket
		socketClient = socket(AF_INET, SOCK_STREAM, 0);

		// check if socket was created
		if (socketClient == -1) {

			// Error during socket creation
			return false;

		}

		// Zero the server address structure
		bzero(&server, sizeof(sockaddr_in));
		// Server port
		server.sin_family	= AF_INET;
		server.sin_addr.s_addr	= inet_addr(serverAddress);
		server.sin_port		= htons(serverPort);

		// Connect to remote server
		if (::connect(socketClient, reinterpret_cast<sockaddr*>(&server), sizeof(server)) < 0) {

			// Error during connection
			return false;

		}

		// Connection to server - success
		isConnected = true;

		// Success
		return true;

	}

	// Disconnect from server
	bool clienttcp::disconnect() {

		// Close connection with server
		if (close(socketClient) < 0) {

			// Error during socket closing
			return false;

		}

		// Disconnected
		isConnected = false;

		// Success
		return true;

	}


	// Send message to server
	bool clienttcp::send(const char* msg, const int size) {

		// Writed message size
		int writed = 0;

		// Send some data to server
		if ((writed = write(socketClient, msg, size)) < 0) {

			// Error during write
			return false;

		// Server disconnected us
		} else if (writed == 0) {

			// Disconnected
			isConnected = false;

			// Disconnected from server
			return false;

		}

		// Success
		return true;

	}

	// Receive message from server
	bool clienttcp::receive(char* msg, const int size) {

		// Readed message size
		int readed = 0;

		// Read reply from server
		if ((readed = read(socketClient, msg, size)) < 0) {

			// Error during read from server
			return false;

		// Server disconnected us
		} else if (readed == 0) {

			// Disconnected
			isConnected = false;

			// Disconnected from server
			return false;

		}

		// Return readed size
		//size = readed;

		// Success
		return true;

	}

	// Is client connected to server
	bool clienttcp::connected() {

		return isConnected;

	}


}	// namespcae networking


// Main client thread
int main(int argc, const char* argv[]) {

	// Client message holder
	std::unique_ptr<char[]>	messageClient(new char[BUFFER_SIZE]);
	// Server message holder
	std::unique_ptr<char[]>	messageServer(new char[BUFFER_SIZE]);

	// Client creation
	networking::clienttcp c;

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


