#pragma once
#include "Connections.h"
#include <iostream>

namespace Pong
{
	namespace Internet
	{
		class TcpSocketConnection: public Connection
		{
		public:
			typedef unsigned char byte;

			virtual std::vector<byte> ReadAllBytes() {};
			virtual std::vector<byte> ReadBytesToDelimiter(byte delimiter) {};
			virtual void SendBytes(std::vector<byte>) {};
			virtual unsigned GetId() {};

			virtual void SetDisconnectAction(std::function<void(Connection*)>) {};
			virtual void SetTransmissionErrorAction(std::function<void(Connection*)>) {};
		};

		class TcpSocketServer: public Server
		{
		public:
			virtual void StartListeningAsync() {};
			virtual void StartListening() { std::cout << "Hello. Im server"; };
			virtual void StopListening() { };
			virtual bool IsAsync() { return false; };
			virtual void SetConnectAction(std::function<void(std::shared_ptr<Connection>)>) {};
		};
	}
}