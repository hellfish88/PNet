#include "Socket.h"
#include <assert.h>
#include <iostream>

namespace PNet {

	PNet::Socket::Socket(IPVersion ipversion, SocketHandle handle)
		: ipversion(ipversion), handle(handle){
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);
	}

	PResult PNet::Socket::Create(){
		assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);

		if (handle != INVALID_SOCKET) {
			return PResult::P_GenericError;
		}

		handle = socket((ipversion == IPVersion::IPv4) ? AF_INET : AF_INET6, SOCK_STREAM, IPPROTO_TCP); // Attempt to create a socket

		if (handle == INVALID_SOCKET) {
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != PResult::P_Success) {
			return PResult::P_GenericError;
		}

		return PResult::P_Success;
	}

	PResult PNet::Socket::Close(){

		if (handle == INVALID_SOCKET) {
			return PResult::P_GenericError;
		}
		int result = closesocket(handle);

		if (result != 0) {
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		handle = INVALID_SOCKET;
		return PResult::P_Success;
	}

	SocketHandle PNet::Socket::GetHandle()
	{
		return handle;
	}

	IPVersion PNet::Socket::GetIPVersion()
	{
		return ipversion;
	}

	PResult Socket::Bind(IPEndpoint endpoint) {
		assert(ipversion == endpoint.GetIPVersion());

		if (ipversion == IPv4) {
			sockaddr_in addr = endpoint.GetSockAddrIPv4();
			int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));

			if (result != 0) {
				int error = WSAGetLastError();
				return PResult::P_GenericError;
			}
		} else {
			sockaddr_in6 addr = endpoint.GetSockAddrIPv6();
			int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in6));

			if (result != 0) {
				int error = WSAGetLastError();
				return PResult::P_GenericError;
			}
		}




		return PResult::P_Success;
	}

	PResult Socket::Listen(IPEndpoint endpoint, int backlog) {

		if (ipversion == IPv6) {
			if (SetSocketOption(SocketOption::IPV6_Only, FALSE) != P_Success) {
				return P_GenericError;
			}
		}


		if (Bind(endpoint) != P_Success) {
			return P_GenericError;
		}

		int result = listen(handle, backlog);

		if (result != 0) {
			int error = WSAGetLastError();
			return P_GenericError;
		}
		return P_Success;
	}

	PResult Socket::Accept(Socket& outSocket) {
		assert(ipversion == IPv4 || ipversion == IPv6);

		if (ipversion == IPv4) {
			sockaddr_in addr = {};
			int len = sizeof(sockaddr_in);
			SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)(&addr), &len);
			if (acceptedConnectionHandle == INVALID_SOCKET) {
				int error = WSAGetLastError();
				return P_GenericError;
			}

			IPEndpoint newConnectionEndpoint((sockaddr*)&addr);
			std::cout << "New connection accepted!" << std::endl;
			newConnectionEndpoint.Print();
			outSocket = Socket(IPv4, acceptedConnectionHandle);
		} else {
			sockaddr_in6 addr = {};
			int len = sizeof(sockaddr_in6);
			SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)(&addr), &len);
			if (acceptedConnectionHandle == INVALID_SOCKET) {
				int error = WSAGetLastError();
				return P_GenericError;
			}

			IPEndpoint newConnectionEndpoint((sockaddr*)&addr);
			std::cout << "New connection accepted!" << std::endl;
			newConnectionEndpoint.Print();
			outSocket = Socket(IPv6, acceptedConnectionHandle);
		}




		return P_Success;
	}

	PResult Socket::Connect(IPEndpoint endpoint) {

		assert(ipversion == endpoint.GetIPVersion());
		int result = 0;
		if (ipversion == IPv4) {
			sockaddr_in addr = endpoint.GetSockAddrIPv4();
			result = connect(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
		} else {
			sockaddr_in6 addr = endpoint.GetSockAddrIPv6();
			result = connect(handle, (sockaddr*)(&addr), sizeof(sockaddr_in6));
		}

		if (result != 0) {
			int error = WSAGetLastError();
			return P_GenericError;
		}
		return P_Success;
	}

	PResult Socket::Send(const void* data, int numberOfBytes, int& bytesSent) {

		bytesSent = send(handle, (const char*)data, numberOfBytes, NULL);

		if (bytesSent == SOCKET_ERROR) {
			int error = WSAGetLastError();
			return P_GenericError;
		}

		return P_Success;
	}

	PResult Socket::Recv(void* destination, int numberOfBytes, int& bytesReceived) {
		bytesReceived = recv(handle, (char*)destination, numberOfBytes, NULL);
		if (bytesReceived == 0) {
			return P_GenericError;
		}

		if (bytesReceived == SOCKET_ERROR) {
			int error = WSAGetLastError();
			return P_GenericError;
		}

		return P_Success;
	}

	PResult Socket::Send(Packet& packet) {

		uint16_t encodedPacketSize = htons(packet.buffer.size());
		PResult result = SendAll(&encodedPacketSize, sizeof(uint16_t));

		if (result != P_Success) {
			return P_GenericError;
		}

		result = SendAll(packet.buffer.data(), packet.buffer.size());
		if (result != P_Success) {
			return P_GenericError;
		}

		return P_Success;
	}

	PResult Socket::Recv(Packet& packet) {
		packet.Clear();

		uint16_t encodedSize = 0;
		PResult result = RecvAll(&encodedSize, sizeof(uint16_t));
		if (result != P_Success) {
			return P_GenericError;
		}

		uint16_t bufferSize = ntohs(encodedSize);

		if (bufferSize > g_MaxPacketSize)
			return P_GenericError;


		packet.buffer.resize(bufferSize);
		result = RecvAll(&packet.buffer[0], bufferSize);
		if (result != P_Success) {
			return P_GenericError;
		}

		return P_Success;
	}

	PResult Socket::SendAll(const void* data, int numberOfBytes) {
		int totalBytesSent = 0;

		while (totalBytesSent < numberOfBytes) {
			int bytesRemaining = numberOfBytes - totalBytesSent;
			int bytesSent = 0;

			char* bufferOfset = (char*)data + totalBytesSent;

			PResult result = Send(bufferOfset, bytesRemaining, bytesSent);

			if (result != P_Success) {
				return P_GenericError;
			}
			totalBytesSent += bytesSent;
		}

		return P_Success;
	}

	PResult Socket::RecvAll(void* destination, int numberOfBytes) {
		int totalBytesReceived = 0;

		while (totalBytesReceived < numberOfBytes) {
			int bytesRemaining = numberOfBytes - totalBytesReceived;
			int bytesReceived = 0;

			char* bufferOfset = (char*)destination + totalBytesReceived;

			PResult result = Recv(bufferOfset, bytesRemaining, bytesReceived);

			if (result != P_Success) {
				return P_GenericError;
			}
			totalBytesReceived += bytesReceived;
		}

		return P_Success;
	}

	PResult Socket::SetSocketOption(SocketOption option, BOOL value) {

		int result{ 0 };
		switch (option) {
			case SocketOption::TCP_NoDelay:
				result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char*)&value, sizeof(value));
				break;
			case SocketOption::IPV6_Only:
				result = setsockopt(handle, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&value, sizeof(value));
				break;
			default:
				return PResult::P_GenericError;
		}

		if (result != 0) {
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}
		return PResult::P_Success;
	}

}