#pragma	once
#ifndef	NETWORKING_UDP_CLIENT_HPP
#define	NETWORKING_UDP_CLIENT_HPP


#include <arpa/inet.h>

#include "udpsocket.hpp"


// Networking namespace
namespace networking {


	// UDP Client class
	class udpclient {

		sockaddr_in	server;			// Server address

		udpsocket_t	socketClient;		// Client socket

		bool		isConnected;		// Is connected to server


	public:

		// C-tor
		udpclient();
		// C-tor
		udpclient(const char* serverAddress, const unsigned int serverPort);
		// D-tor
		~udpclient();

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


}	// namespace networking


#endif	// NETWORKING_UDP_CLIENT_HPP

