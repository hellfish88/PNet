#pragma once
#include "IPVersion.h"
#include <string>
#include <vector>
#include <WS2tcpip.h>

namespace PNet {
	class IPEndpoint {
		public:
			IPEndpoint(const char* ip, unsigned short port);
			IPEndpoint(sockaddr* addr);
			IPVersion GetIPVersion();
			std::vector<uint8_t> getIPBytes();
			std::string GetHostname();
			std::string GetIPString();
			unsigned short GetPort();
			sockaddr_in GetSockAddrIPv4();
			sockaddr_in6 GetSockAddrIPv6();
			void Print();
		private:
			IPVersion ipversion{ IPVersion::Unknown };
			std::string hostname{ "" };
			std::string ip_string{ "" };
			std::vector<uint8_t> ip_bytes;
			unsigned short port{ 0 };
	};
}