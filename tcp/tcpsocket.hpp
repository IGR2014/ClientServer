#pragma	once
#ifndef	NETWORKING_TCP_SOCKET_HPP
#define	NETWORKING_TCP_SOCKET_HPP


// Networking namespace
namespace networking {


	// TCP Socket class
	class tcpsocket_t {

		int	unixSocket;		// Unix socket handle type


	public:

		// C-tor
		tcpsocket_t();
		// C-tor from platform
		template<typename T>
		explicit tcpsocket_t(T&);
		// D-tor
		~tcpsocket_t();

		// Open
		bool	open();
		// Close
		bool	close();

		// Send
		int	send(const char* msg, const int size) const;
		// Receive
		int	receive(char* msg, const int size) const;

		// Set from platform implementation
		template<typename T>
		void	fromImpl(T&);
		// Get platform implementation
		template<typename T>
		T&	toImpl();


	};


	// C-tor from platform
	template<typename T>
	tcpsocket_t::tcpsocket_t(T &soc) {

		fromImpl<T>(soc);

	}


	// Set from platform implementation
	template<typename T>
	void tcpsocket_t::fromImpl(T &soc) {

		unixSocket = static_cast<int>(soc);

	}

	// Get platform implementation
	template<typename T>
	T& tcpsocket_t::toImpl() {

		// Cast to pointer to required type
		return static_cast<T&>(unixSocket);

	}


}	// namespace networking


#endif	// NETWORKING_TCP_SOCKET_HPP


