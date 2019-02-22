#pragma	once
#ifndef	NETWORKING_TCP_CLIENT_HPP
#define	NETWORKING_TCP_CLIENT_HPP


#include <arpa/inet.h>
#include "../socket.hpp"


// Networking namespace
namespace networking {


	// TCP Client class
	class tcpclient {

		sockaddr_in	server;			// Server address

		socket_t	socketClient;		// Client socket

		bool		isConnected;		// Is connected to server


	public:

		// C-tor
		tcpclient();
		// C-tor
		tcpclient(const char* serverAddress, const unsigned int serverPort);
		// D-tor
		~tcpclient();

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


#endif	// NETWORKING_TCP_CLIENT_HPP

