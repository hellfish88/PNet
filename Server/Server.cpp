#include <IncludeMe.h>
#include <iostream>

using namespace PNet;


bool ProcessPacket(Packet& packet) {

	switch (packet.GetPacketType()) {
	case PT_ChatMessage: {
		std::string chatmessage;
		packet >> chatmessage;
		std::cout << "Chat message: " << chatmessage << std::endl;
		break;

	}
	case PT_IntegerArray: {
		uint32_t arraySize{ 0 };
		packet >> arraySize;
		std::cout << "Array size: " << arraySize << std::endl;
		for (uint32_t i = 0; i < arraySize; i++) {
			uint32_t element = 0;
			packet >> element;
			std::cout << "Element[" << i << "]" << element << std::endl;
		}
		break;
	}
	case PT_Invalid: {
		break;
	}
	default:
		return false;
	}

	return true;
}



int main() {

	if (Network::Initialize()) {

		std::cout << "Winsock api successfully initialized" << std::endl;

		//IPEndpoint test("www.google.com", 1337);
		//if (test.GetIPVersion() == IPVersion::IPv4) {
		//	std::cout << "Hostname: " << test.GetHostname() << std::endl;
		//	std::cout << "IP: " << test.GetIPString() << std::endl;
		//	std::cout << "Port: " << test.GetPort() << std::endl;
		//	std::cout << "IP Bytes..." << std::endl;
		//	for (auto& digit : test.getIPBytes()) {
		//		std::cout << (int)digit << std::endl;
		//	}
		//} else {
		//	std::cerr << "This is not a valid IPv4 address" << std::endl;
		//}

		// server to listen for connections on port XYz
		// socket - bind  to port




		Socket socket(IPv6);

		if (socket.Create() == PResult::P_Success) {
			std::cout << "Socket6 successfully created" << std::endl;
			if (socket.Listen(IPEndpoint("::1", 1337)) == PResult::P_Success) {
				std::cout << "Socket6 successfully listening to port" << std::endl;
				Socket newConnection;
				if (socket.Accept(newConnection) == P_Success) {
					std::cout << "New connection accepted" << std::endl;

					std::string buffer{ "" };

					Packet packet;


					while (true) {

						PResult result = newConnection.Recv(packet);

						if (result != P_Success)
							break;
						
						if (!ProcessPacket(packet))
							break;

					}

					newConnection.Close();
				} else {
					std::cerr << "Failed to accept new conn" << std::endl; 
				}
			} else {
				std::cerr << "Failed to bind socket to port" << std::endl;
			}
			socket.Close();
		}
		else {
			std::cerr << "Socket failed to create" << std::endl;
		}

	}

	Network::Shutdown();
	return 0;
}