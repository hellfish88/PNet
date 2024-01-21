#include "PacketManager.h"

namespace PNet {

	void PNet::PacketManager::Clear() {
		packets = std::queue<std::shared_ptr<Packet>>{};
	}

	bool PNet::PacketManager::HasPendingPackets() {
		return (!packets.empty()); // returns true if packets are pending
	}

	void PNet::PacketManager::Append(std::shared_ptr<Packet> p) {
		packets.push(std::move(p)); // add packet to q
	}

	std::shared_ptr<Packet> PNet::PacketManager::Retrieve() {
		std::shared_ptr<Packet> p = packets.front();
		return p;
	}

	void PNet::PacketManager::Pop() {
		packets.pop();
	}

}