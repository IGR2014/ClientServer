#pragma	once
#ifndef	NETWORKING_UDP_SERVER_HPP
#define	NETWORKING_UDP_SERVER_HPP


#include <arpa/inet.h>
#include "../socket.hpp"


// Networking namespace
namespace networking {


	// UDP server class
	class udpserver {

		sockaddr_in	server;			// Server address

		socket_t	socketServer;		// Server socket

		bool		isRunning;		// Is server running


	public:

		// C-tor
		udpserver();
		// C-tor
		explicit udpserver(const unsigned int serverPort);
		// C-tor
		udpserver(const unsigned int serverPort, const unsigned int maxClients);
		// D-tor
		~udpserver();

		// Start server
		bool		start(const unsigned int serverPort);
		// Start server
		bool		start(const unsigned int serverPort, const unsigned int maxClients);
		// Stop server
		bool		stop();

		// Send message to client (blocking)
		bool		send(const socket_t &clientID, const char* msg, const int size);
		// Receive message from client (blocking)
		bool		receive(const socket_t &clientID, char* msg, const int size);

		// Wait for connection (blocking)
		socket_t*	waitForConnect();

		// Is server running
		bool		running();


	};


}	// networking namespace


#endif	// NETWORKING_UDP_SERVER_HPP

