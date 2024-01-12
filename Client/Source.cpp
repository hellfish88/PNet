#include <IncludeMe.h>
#include <iostream>
#include "Client.h"

using namespace PNet;

int main() {

	Client client;

	if (client.Connect(IPEndpoint("::1", 1337))) {
		while (client.IsConnected()) {
			client.Frame();
		}
	}

	Network::Shutdown();
	system("pause");
	return 0;
}