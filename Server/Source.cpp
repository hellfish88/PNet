//#include <IncludeMe.h>
#include <iostream>
#include "MyServer.h"



int main() {

	if (Network::Initialize()) {


		MyServer server;

		if (server.Initialize(IPEndpoint("::", 1337))) {

			while (true) {
				server.Frame();
			}
		}
	}
	Network::Shutdown();
	system("Pause");

	return 0;
}