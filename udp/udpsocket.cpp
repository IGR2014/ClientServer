#include <unistd.h>	
#include <sys/socket.h>	
#include <arpa/inet.h>

#include "udpsocket.hpp"


// Networking namespace
namespace networking {


	// C-tor
	udpsocket_t::udpsocket_t()
		: unixSocket(-1) {}

	// D-tor
	udpsocket_t::~udpsocket_t() {

		// Close socket if opened
		close();

	}


	// Open
	bool udpsocket_t::open() {

		// Try open socket
		if ((unixSocket = ::socket(AF_INET, SOCK_DGRAM, 0)) < 0) {

			// Error
			return false;

		}

		// Success
		return true;

	}

	// Close
	bool udpsocket_t::close() {

		// Close socket
		if (::close(unixSocket) < 0) {

			// Error
			return false;

		}

		// Success
		return true;

	}


	// Send
	int udpsocket_t::send(const char* msg, const int size) const {

		// Destination address
		sockaddr_in	destAddr;
		// Destination address size
		socklen_t	destAddrSize	= sizeof(sockaddr_in);

		// Try write to socket
		return ::sendto(unixSocket, msg, size, 0, reinterpret_cast<sockaddr*>(&destAddr), destAddrSize);

	}

	// Receive
	int udpsocket_t::receive(char* msg, const int size) const {

		// Destination address
		sockaddr_in	destAddr;
		// Destination address size
		socklen_t	destAddrSize	= sizeof(sockaddr_in);

		// Try read from socket
		return ::recvfrom(unixSocket, msg, size, 0, reinterpret_cast<sockaddr*>(&destAddr), &destAddrSize);

	}


}	// namespace networking


