/**
 * @file UdpSocket.cpp
 * @brief 
 */

#include <unistd.h>
#include <string.h>
#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "UdpSocket.h"

namespace NetMan {

UdpSocket::UdpSocket() {

	this->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

	if(this->fd < 0) {
		throw std::runtime_error("socket() failed");
	}
}

void UdpSocket::sendPacket(void *data, u32 size, const char *ip, u16 port) {

	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(sockaddr_in));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	dest.sin_addr.s_addr = inet_addr(ip);

	sendto(this->fd, data, size, 0, (const struct sockaddr*)&dest, sizeof(dest));
}

void UdpSocket::recvPacket(void *data, u32 size, const char *ip, u16 port) {

	struct sockaddr_in src;
	in_addr_t src_ip = inet_addr(ip);
	u16 src_port = htons(port);
	socklen_t src_len = sizeof(src);

	do {
		recvfrom(this->fd, data, size, 0, (struct sockaddr*)&src, &src_len);
	} while(src.sin_addr.s_addr != src_ip && src.sin_port != src_port);
}

UdpSocket::~UdpSocket() {
	close(this->fd);
}

}
