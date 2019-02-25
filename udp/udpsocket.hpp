#pragma	once
#ifndef	NETWORKING_UDP_SOCKET_HPP
#define	NETWORKING_UDP_SOCKET_HPP


// Networking namespace
namespace networking {


	// UDP Socket class
	class udpsocket_t {

		int	unixSocket;		// Unix socket handle type


	public:

		// C-tor
		udpsocket_t();
		// C-tor from platform
		template<typename T>
		explicit udpsocket_t(T&);
		// D-tor
		~udpsocket_t();

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
	udpsocket_t::udpsocket_t(T &soc) {

		fromImpl<T>(soc);

	}


	// Set from platform implementation
	template<typename T>
	void udpsocket_t::fromImpl(T &soc) {

		unixSocket = static_cast<int>(soc);

	}

	// Get platform implementation
	template<typename T>
	T& udpsocket_t::toImpl() {

		// Cast to pointer to required type
		return static_cast<T&>(unixSocket);

	}


}	// namespace networking


#endif	// NETWORKING_UDP_SOCKET_HPP


