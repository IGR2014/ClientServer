#pragma	once
#ifndef	NETWORKING_SOCKET_HPP
#define	NETWORKING_SOCKET_HPP


// Networking namespace
namespace networking {


	// Socket class
	class socket_t {

		int	unixSocket;		// Unix socket handle type


	public:

		// C-tor
		socket_t();
		// C-tor from platform
		template<typename T>
		explicit socket_t(T&);
		// D-tor
		~socket_t();

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
	socket_t::socket_t(T &soc) {

		fromImpl<T>(soc);

	}


	// Set from platform implementation
	template<typename T>
	void socket_t::fromImpl(T &soc) {

		unixSocket = static_cast<int>(soc);

	}

	// Get platform implementation
	template<typename T>
	T& socket_t::toImpl() {

		// Cast to pointer to required type
		return static_cast<T&>(unixSocket);

	}


}	// namespace networking


#endif	// NETWORKING_SOCKET_HPP


