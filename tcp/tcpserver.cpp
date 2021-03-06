#include <iostream>
#include <thread>
#include <memory>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "tcpserver.hpp"


// Networking namespace
namespace networking {


	// Maximum clients number
	static const unsigned int	MAX_CLIENTS_NUMBER	= 1024;


	// C-tor
	tcpserver::tcpserver()
		: server(),
		  socketServer(),
		  isRunning(false) {}

	// C-tor
	tcpserver::tcpserver(const unsigned int serverPort)
		: server(),
		  socketServer(),
		  isRunning(false) {

		// Start server on required port with default maximum clients number
		start(serverPort, MAX_CLIENTS_NUMBER);

	}

	// C-tor
	tcpserver::tcpserver(const unsigned int serverPort, const unsigned int maxClients)
		: server(),
		  socketServer(),
		  isRunning(false) {

		// Start server on required port
		start(serverPort, maxClients);

	}

	// D-tor
	tcpserver::~tcpserver() {

		// Stop server if running
		if (isRunning) {

			stop();

		}

	}


	// Start server
	bool tcpserver::start(const unsigned int serverPort) {

		// Start with default maximum clients number
		return start(serverPort, MAX_CLIENTS_NUMBER);

	}

	// Start server
	bool tcpserver::start(const unsigned int serverPort, const unsigned int maxClients) {

		// Create TCP socket
		if (!socketServer.open()) {

			// Error during socket creation
			return false;

		}

		// Multiclient flag
		int multiclient = 1;
		// Enable multiclient connect
		if (::setsockopt(socketServer.toImpl<int>(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &multiclient, sizeof(int)) < 0) {

			// Error during socket options setup
			return false;

		}

		// Zero the server address structure
		bzero(&server, sizeof(sockaddr_in));
		// Prepare the server address structure
		server.sin_family	= AF_INET;
		server.sin_addr.s_addr	= INADDR_ANY;
		server.sin_port		= htons(serverPort);

		// Bind socket to port
		if (::bind(socketServer.toImpl<int>(), reinterpret_cast<sockaddr*>(&server), sizeof(server)) < 0) {

			// Error during binding to port
			return false;

		}
	
		// Listen for port (maximum N connections)
		if (::listen(socketServer.toImpl<int>(), maxClients) < 0) {

			// Error during port listening start
			return false;

		}

		// Server started
		isRunning = true;

		// Success
		return true;

	}

	// Stop server
	bool tcpserver::stop() {

		// Server stoped
		isRunning = false;

		// Close connection
		if (!socketServer.close()) {

			// Error during connection close
			return false;

		}

		// Success
		return true;

	}


	// Send message to client (blocking)
	bool tcpserver::send(const tcpsocket_t &clientID, const char* msg, const int size) {

		// Writed message size
		int writed = 0;

		// Send some data to client
		if ((writed = clientID.send(msg, size)) < 0) {

			// Error during write
			return false;

		// Client disconnected
		} else if (writed == 0) {

			// No client
			return false;

		}

		// Success
		return true;

	}

	// Receive message from client (blocking)
	bool tcpserver::receive(const tcpsocket_t &clientID, char* msg, const int size) {

		// Readed message size
		int readed = 0;

		// Read reply from client
		if ((readed = clientID.receive(msg, size)) < 0) {

			// Error during read
			return false;

		// Client disconnected
		} else if (readed == 0) {

			// No client
			return false;

		}

		// Return readed size
		//size = readed;

		// Success
		return true;

	}


	// Wait for connection (blocking)
	tcpsocket_t* tcpserver::waitForConnect() {

		int		socketClient	= -1;			// Client socket
		sockaddr_in	client;					// Client address
		socklen_t	clientLength	= sizeof(sockaddr_in);	// Client address size

		// Wait for connection and accept it
		if ((socketClient = ::accept(socketServer.toImpl<int>(), reinterpret_cast<sockaddr*>(&client), static_cast<socklen_t*>(&clientLength))) > 0) {

			// New client added
			std::cout << "New client connected" << std::endl;

		}

		// Client acception failed
		return new tcpsocket_t(socketClient);

	}


	// Is server running
	bool tcpserver::running() {

		return isRunning;

	}


}	// namespcae networking

