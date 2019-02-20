#include <iostream>
#include <memory>
#include <thread>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>


// Default port number
static const unsigned int DEFAULT_PORT_NUMBER		= 5000;
// Default number of clients
static const unsigned int DEFAULT_CLIENTS_NUMBER	= 10;


// Server class
class server {

	sockaddr_in	handshakeAddr	{};	// Server handshake address

	std::thread	servThread;		// Server thread
	std::thread	handshakeThread;	// Server handshake thread

	int		handshakefd;		// Server handshake descriptor

	unsigned int	handshakePortNumber;	// Server port number

	bool		isRunning;		// Is server running


public:

	// C-tor
	server();
	// C-tor
	explicit server(unsigned int port);
	// D-tor
	~server();

	// Start
	void		start();
	// Stop
	void		stop();

	// Server thread
	void		serverThread();
	// Server handshake thread
	void		serverHandshakeThread();


};


// C-tor
server::server()
	: handshakefd		(-1),
	  handshakePortNumber	(DEFAULT_PORT_NUMBER),
	  isRunning		(false) {}

// C-tor
server::server(unsigned int port)
	: handshakefd		(-1),
	  handshakePortNumber	(port),
	  isRunning		(false) {}

// D-tor
server::~server() {
}


// Start
void server::start() {

	// Set server to running state
	isRunning = true;

	// Start server thread
	handshakeThread = std::thread(&server::serverHandshakeThread, this);

}

// Stop
void server::stop() {

	// Set server to stopped state
	isRunning = false;

	// Stop server thread
	if (handshakeThread.joinable()) {

		handshakeThread.join();

	}

}


// Server thread
void server::serverThread() {

	sockaddr_in	serv_addr	{};				// Server listener address

	int		listenfd	= -1;				// Server listener descriptor
	int		connectionfd	= -1;				// Server connection descriptor

	unsigned int	portNumber	= handshakePortNumber + 1;	// Server port number

	// Create array for data holding
	std::unique_ptr<unsigned char[]> sendBuff = std::unique_ptr<unsigned char[]>(new unsigned char[1024]);

	// Listener file descriptor
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

		std::cout << "ERROR\tUnable to open listener socket" << std::endl;
		return;

	}

	// Cleanup server structures
	bzero(reinterpret_cast<char*>(&serv_addr), sizeof(serv_addr));

	// Setup sevrev socket info
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	serv_addr.sin_port		= htons(portNumber);

	// Bind server to required port
	if (bind(listenfd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {

		std::cout << "ERROR\tUnable to bind port " << portNumber << std::endl;
		std::cout << strerror(errno) << std::endl;
		return;

	}

	// Listen for connection (maximum 10)
	if (listen(listenfd, DEFAULT_CLIENTS_NUMBER) < 0) {

		std::cout << "ERROR\tUnable to listen to socket" << std::endl;
		return;

	}

	// Accept incoming connection
	if ((connectionfd = accept(listenfd, nullptr, nullptr)) < 0) {

		std::cout << "ERROR\tUnable to accept connection" << std::endl;
		return;

	}

	// Write message
	memcpy(sendBuff.get(), "SERVER HELLO!", 13);

	// Write data to soccet
	if (write(connectionfd, sendBuff.get(), 13) < 0) {

		std::cout << "ERROR\tUnable to write connection" << std::endl;
		return;

	}

	// Close connection
	if (close(connectionfd) < 0) {

		std::cout << "ERROR\tUnable to close connection" << std::endl;
		return;

	}

	// Close connection
	if (close(listenfd) < 0) {

		std::cout << "ERROR\tUnable to close socket" << std::endl;
		return;

	}

}


// Server handshake thread
void server::serverHandshakeThread() {

	// Create array for data holding
	std::unique_ptr<char[]> sendBuff = std::unique_ptr<char[]>(new char[1024]);

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

	// Setup sevrev socket info
	handshakeAddr.sin_family	= AF_INET;
	handshakeAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	handshakeAddr.sin_port		= htons(handshakePortNumber);

	// Bind server to required port
	if (bind(handshakefd, reinterpret_cast<sockaddr*>(&handshakeAddr), sizeof(handshakeAddr)) < 0) {

		std::cout << "ERROR\tUnable to bind handshake port " << handshakePortNumber << std::endl;
		return;

	}

	// Main server loop
	while (isRunning) {

		int		readed	= 0;
		unsigned int	length	= sizeof(handshakeAddr);

		// Wait for data from connected clients
		if ((readed = recvfrom(handshakefd, sendBuff.get(), 1024, 0, reinterpret_cast<sockaddr*>(&handshakeAddr), &length)) < 0) {

			std::cout << "ERROR\tReceive data from broadcast" << std::endl;
			return;

		}

		// Output string if something readed
		if (readed > 0) {

			std::cout << sendBuff.get() << std::endl;

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

	// Server object
	server s;

	// Server started
	s.start();

	// Wait
	std::this_thread::sleep_for(std::chrono::seconds(1000));

	// Server stopped
	s.stop();

	return 0;

}
