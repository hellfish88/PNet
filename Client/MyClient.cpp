#include "MyClient.h"
#include <iostream>

bool MyClient::ProcessPacket(std::shared_ptr<Packet> packet) {
	switch (packet->GetPacketType()) {
	case PT_ChatMessage: {
		std::string chatmessage;
		*packet >> chatmessage;
		std::cout << "Chat message: " << chatmessage << std::endl;
		break;

	}
	case PT_IntegerArray: {
		uint32_t arraySize{ 0 };
		*packet >> arraySize;
		std::cout << "Array size: " << arraySize << std::endl;
		for (uint32_t i = 0; i < arraySize; i++) {
			uint32_t element = 0;
			*packet >> element;
			std::cout << "Element[" << i << "]" << element << std::endl;
		}
		break;
	}
	case PT_Invalid: {
		break;
	}
	default:
		std::cout << "Unrecognized packet type: " << packet->GetPacketType() << std::endl;
		return false;
	}

}

void MyClient::OnConnect() {

	std::cout << "Successfully conncted" << std::endl;
	std::shared_ptr<Packet> helloPacket = std::make_unique<Packet>(PacketType::PT_ChatMessage);
	*helloPacket << std::string("Client say hello");
	connection.pm_outgoing.Append(helloPacket);
}
