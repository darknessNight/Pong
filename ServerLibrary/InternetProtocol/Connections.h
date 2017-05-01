#pragma once
#include <vector>
#include <memory>
#include <functional>

#ifndef _MSC_VER
#define abstract
#endif

namespace Pong
{
	namespace Internet
	{
		class Connection abstract
		{
		public:
			virtual ~Connection() = default;
			typedef unsigned char byte;

			virtual std::vector<byte> ReadAllBytes() = 0;
			virtual std::vector<byte> ReadBytesToDelimiter(byte delimiter) = 0;
			virtual std::vector<byte> ReadBytes(unsigned size) = 0;
			virtual void SendBytes(std::vector<byte> bytes) = 0;
			virtual unsigned GetId() = 0;
			virtual bool IsSomethingToReceive() = 0;

			virtual void Abort() = 0;

			virtual void SetDisconnectAction(std::function<void(Connection*)>) = 0;
			virtual void SetTransmissionErrorAction(std::function<void(Connection*)>) = 0;
		};

		class Server abstract
		{
		public:
			virtual void StartListeningAsync(int port) = 0;
			virtual void StartListening(int port) = 0;
			virtual void StopListening() = 0;
			virtual bool IsAsync() = 0;
			virtual void SetConnectAction(std::function<void(std::shared_ptr<Connection>)>) = 0;
			virtual ~Server() = default;
		};
	}
}