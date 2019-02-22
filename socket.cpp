#include <unistd.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>

#include "socket.hpp"


// Networking namespace
namespace networking {


	// C-tor
	socket_t::socket_t()
		: unixSocket(-1) {}

	// D-tor
	socket_t::~socket_t() {

		// Close socket if opened
		close();

	}


	// Open
	bool socket_t::open() {

		// Try open socket
		if ((unixSocket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {

			// Error
			return false;

		}

		// Success
		return true;

	}

	// Close
	bool socket_t::close() {

		// Close socket
		if (::close(unixSocket) < 0) {

			// Error
			return false;

		}

		// Success
		return true;

	}


	// Send
	int socket_t::send(const char* msg, const int size) const {

		// Try write to socket
		return ::write(unixSocket, msg, size);

	}

	// Receive
	int socket_t::receive(char* msg, const int size) const {

		// Try read from socket
		return ::read(unixSocket, msg, size);

	}


}	// namespace networking


