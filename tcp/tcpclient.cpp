#include <iostream>
#include <memory>
	
#include <cstring>
#include <unistd.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>

#include "tcpclient.hpp"


// Networking namespace
namespace networking {


	// C-tor
	tcpclient::tcpclient()
		: server	(),
		  socketClient	(-1),
		  isConnected	(false) {}

	// C-tor
	tcpclient::tcpclient(const char* serverAddress, const unsigned int serverPort)
		: server	(),
		  socketClient	(-1),
		  isConnected	(false) {

		// Connect to server
		connect(serverAddress, serverPort);

	}

	// D-tor
	tcpclient::~tcpclient() {

		// Close connection if connected
		if (isConnected) {

			disconnect();

		}

	}


	// Connect to server
	bool tcpclient::connect(const char* serverAddress, const unsigned int serverPort) {

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
	bool tcpclient::disconnect() {

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
	bool tcpclient::send(const char* msg, const int size) {

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
	bool tcpclient::receive(char* msg, const int size) {

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
	bool tcpclient::connected() {

		return isConnected;

	}


}	// namespcae networking

