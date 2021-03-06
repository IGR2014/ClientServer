#pragma	once
#ifndef	NETWORKING_TCP_SERVER_HPP
#define	NETWORKING_TCP_SERVER_HPP


#include <arpa/inet.h>

#include "tcpsocket.hpp"


// Networking namespace
namespace networking {


	// TCP server class
	class tcpserver {

		sockaddr_in	server;			// Server address

		tcpsocket_t	socketServer;		// Server socket

		bool		isRunning;		// Is server running


	public:

		// C-tor
		tcpserver();
		// C-tor
		explicit tcpserver(const unsigned int serverPort);
		// C-tor
		tcpserver(const unsigned int serverPort, const unsigned int maxClients);
		// D-tor
		~tcpserver();

		// Start server
		bool		start(const unsigned int serverPort);
		// Start server
		bool		start(const unsigned int serverPort, const unsigned int maxClients);
		// Stop server
		bool		stop();

		// Send message to client (blocking)
		bool		send(const tcpsocket_t &clientID, const char* msg, const int size);
		// Receive message from client (blocking)
		bool		receive(const tcpsocket_t &clientID, char* msg, const int size);

		// Wait for connection (blocking)
		tcpsocket_t*	waitForConnect();

		// Is server running
		bool		running();


	};


}	// networking namespace


#endif	// NETWORKING_TCP_SERVER_HPP

