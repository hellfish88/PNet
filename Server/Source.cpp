//#include <IncludeMe.h>
#include <iostream>
#include "Server.h"

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

	Server server;

	if (server.Initialize(IPEndpoint("::", 1337))) {

		while (true) {
			server.Frame();
		}
	}

	Network::Shutdown();
	system("Pause");

	return 0;
}

