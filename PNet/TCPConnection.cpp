#include "TCPConnection.h"


namespace PNet {
	PNet::TCPConnection::TCPConnection(Socket socket, IPEndpoint endpoint) 
	 :socket(socket), endpoint(endpoint){
		stringRepresentation = "[" + endpoint.GetIPString();
		stringRepresentation += ":" + std::to_string(endpoint.GetPort()) + "]";
	}

	void PNet::TCPConnection::Close() {
		socket.Close();
	}

	std::string PNet::TCPConnection::ToString() {
		return stringRepresentation;
	}

}