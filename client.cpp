#include <iostream>
#include <thread>
#include <memory>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>


// Default port number
static const unsigned int DEFAULT_PORT_NUMBER		= 5000;


// Client class
class client {

	sockaddr_in	handshakeAddr	{};	// Client handshake address

	std::thread	handshakeThread;	// Client handshake thread

	int		handshakefd;		// Client handshake descriptor

	bool		isRunning;		// Is client running


public:

	// C-tor
	client();
	// C-tor
	explicit client(unsigned int port);
	// D-tor
	~client();

	// Start
	void		start();
	// Stop
	void		stop();

	// Client thread
	void		clientThread();
	// Client handshake thread
	void		clientHandshakeThread();


};


// C-tor
client::client()
	: handshakefd		(-1),
	  isRunning		(false) {}

// C-tor
client::client(unsigned int port)
	: handshakefd		(-1),
	  isRunning		(false) {}

// D-tor
client::~client() {
}


// Start
void client::start() {

	// Set server to running state
	isRunning = true;

	// Start server thread
	handshakeThread = std::thread(&client::clientHandshakeThread, this);

}

// Stop
void client::stop() {

	// Set server to stopped state
	isRunning = false;

	// Stop server thread
	if (handshakeThread.joinable()) {

		handshakeThread.join();

	}

}


// client handshake thread
void client::clientHandshakeThread() {

	// Create array for data holding
	std::unique_ptr<char[]> sendBuff = std::unique_ptr<char[]>(new char[1024] {'M', 'E', 'S', 'S', 'A', 'G', 'E'});

	// Listener file descriptor
	if ((handshakefd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {

		std::cout << "ERROR\tUnable to open handshake socket" << std::endl;
		return;

	}


	// Allow broadcast?
	int broadcast = 1;
	// Make socket available broadcast
	if (setsockopt(handshakefd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {

		std::cout << "ERROR\tUnable to allow socket BROADCAST" << std::endl;
		return;

	}

	// Cleanup server structures
	bzero(reinterpret_cast<char*>(&handshakeAddr), sizeof(handshakeAddr));

	// Remote server info
	hostent* hostinfo	= gethostbyname("255.255.255.255");
	sockaddr_in remoteAddr {};
	remoteAddr.sin_family	= hostinfo->h_addrtype;
	remoteAddr.sin_port	= htons(DEFAULT_PORT_NUMBER);
	memcpy(&remoteAddr.sin_addr.s_addr, hostinfo->h_addr_list[0], hostinfo->h_length);

	// Setup client socket info
	handshakeAddr.sin_family	= AF_INET;
	handshakeAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	handshakeAddr.sin_port		= htons(0);

	// Bind server to required port
	if (bind(handshakefd, reinterpret_cast<sockaddr*>(&handshakeAddr), sizeof(handshakeAddr)) < 0) {

		std::cout << "ERROR\tUnable to bind handshake port " << 0 << std::endl;
		return;

	}

	// Main server loop
	while (isRunning) {

		int writed = 0;

		// Send data to server
		if ((writed = sendto(handshakefd, sendBuff.get(), strlen(sendBuff.get()), 0, reinterpret_cast<sockaddr*>(&remoteAddr), sizeof(remoteAddr))) < 0) {

			std::cout << "ERROR\tSend data to broadcast" << std::endl;
			return;

		}

		// Output string if something readed
		if (writed < 0) {

			std::cout << "ERROR\tNo Server :(" << std::endl;

		}

		// Yield thread
		std::this_thread::yield();

	}

	// Close connection
	if (close(handshakefd) < 0) {

		std::cout << "ERROR\tUnable to close handshake socket" << std::endl;
		return;

	}

}


int main(int argc, char *argv[]) {

	// Client object
	client c;

	// Client started
	c.start();

	// Wait
	std::this_thread::sleep_for(std::chrono::seconds(1000));

	// Client stopped
	c.stop();

	return 0;

} 

