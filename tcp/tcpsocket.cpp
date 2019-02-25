#include <unistd.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>

#include "tcpsocket.hpp"


// Networking namespace
namespace networking {


	// C-tor
	tcpsocket_t::tcpsocket_t()
		: unixSocket(-1) {}

	// D-tor
	tcpsocket_t::~tcpsocket_t() {

		// Close socket if opened
		close();

	}


	// Open
	bool tcpsocket_t::open() {

		// Try open socket
		if ((unixSocket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {

			// Error
			return false;

		}

		// Success
		return true;

	}

	// Close
	bool tcpsocket_t::close() {

		// Close socket
		if (::close(unixSocket) < 0) {

			// Error
			return false;

		}

		// Success
		return true;

	}


	// Send
	int tcpsocket_t::send(const char* msg, const int size) const {

		// Try write to socket
		return ::send(unixSocket, msg, size, 0);

	}

	// Receive
	int tcpsocket_t::receive(char* msg, const int size) const {

		// Try read from socket
		return ::recv(unixSocket, msg, size, 0);

	}


}	// namespace networking


