#pragma once

namespace PNet {
	enum SocketOption {
		TCP_NoDelay, // TRUE = disable Nagle's algorithm
		IPV6_Only // TRUE = ONly ipv6 can connect. False == ipv6 and 6 can both connect. 
	};
}