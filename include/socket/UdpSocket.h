/**
 * @file UdpSocket.h
 * @brief UDP Socket wrapper
 */

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

// Includes C/C++
#include <sys/socket.h>

// Includes 3DS
#include <3ds/types.h>

// Defines
#define DEFAULT_UDP_SOCKET_TIMEOUT_SECS		5
#define DEFAULT_UDP_SOCKET_TIMEOUT_USECS	0

namespace NetMan {

/**
 * @class UdpSocket
 */
class UdpSocket {
	private:
		int fd;					/**< File descriptor for UDP socket */
		struct timeval tv;		/**< Timeout for UDP socket */
		in_addr_t lastOrigin;	/**< Last received packet's origin IP */
		in_port_t lastPort;		/**< Last received packet's origin port */
	public:
		UdpSocket(u32 timeoutSecs = DEFAULT_UDP_SOCKET_TIMEOUT_SECS, u32 timeoutUsecs = DEFAULT_UDP_SOCKET_TIMEOUT_USECS);
		void sendPacket(void *data, u32 size, const std::string &ip, u16 port);
		u32 recvPacket(void *data, u32 size, const std::string &ip = "", u16 port = 0);
		void bindTo(u16 port);
		virtual ~UdpSocket();
};

}

#endif
