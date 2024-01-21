#pragma once
#include <IncludeMe.h>
#include <iostream>

using namespace PNet;

class Server {
	public:
		bool Initialize(IPEndpoint ip);
		void Frame();
	private: 
		Socket listeningSocket;
		bool ProcessPacket(std::shared_ptr<Packet> packet);
		void CloseConnection(int connectionIndex, std::string reason);
		std::vector<TCPConnection> connections;
		std::vector<WSAPOLLFD> master_fd;
		std::vector<WSAPOLLFD> use_fd = master_fd;
};