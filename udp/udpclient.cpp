#include <iostream>
#include <memory>
	
#include <cstring>
#include <unistd.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>

#include "udpsocket.hpp"
#include "udpclient.hpp"


// Networking namespace
namespace networking {


	// C-tor
	udpclient::udpclient()
		: server	(),
		  socketClient	(),
		  isConnected	(false) {}

	// C-tor
	udpclient::udpclient(const char* serverAddress, const unsigned int serverPort)
		: server	(),
		  socketClient	(),
		  isConnected	(false) {

		// Connect to server
		connect(serverAddress, serverPort);

	}

	// D-tor
	udpclient::~udpclient() {

		// Close connection if connected
		if (isConnected) {

			disconnect();

		}

	}


	// Connect to server
	bool udpclient::connect(const char* serverAddress, const unsigned int serverPort) {

		// Create socket
		if (!socketClient.open()) {

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
		if (::connect(socketClient.toImpl<int>(), reinterpret_cast<sockaddr*>(&server), sizeof(server)) < 0) {

			// Error during connection
			return false;

		}

		// Connection to server - success
		isConnected = true;

		// Success
		return true;

	}

	// Disconnect from server
	bool udpclient::disconnect() {

		// Close connection with server
		if (!socketClient.close()) {

			// Error during socket closing
			return false;

		}

		// Disconnected
		isConnected = false;

		// Success
		return true;

	}


	// Send message to server
	bool udpclient::send(const char* msg, const int size) {

		// Writed message size
		int writed = 0;

		// Send some data to server
		if ((writed = socketClient.send(msg, size)) < 0) {

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
	bool udpclient::receive(char* msg, const int size) {

		// Readed message size
		int readed = 0;

		// Read reply from server
		if ((readed = socketClient.receive(msg, size)) < 0) {

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
	bool udpclient::connected() {

		return isConnected;

	}


}	// namespcae networking


