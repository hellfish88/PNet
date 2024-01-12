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

};