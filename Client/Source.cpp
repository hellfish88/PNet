#include <IncludeMe.h>
#include <iostream>
#include "MyClient.h"


int main() {

	if (Network::Initialize()) {

		MyClient client;

		if (client.Connect(IPEndpoint("::1", 1337))) {
			while (client.IsConnected()) {
				client.Frame();
			}
		}
	}
	Network::Shutdown();
	system("pause");
	return 0;
}