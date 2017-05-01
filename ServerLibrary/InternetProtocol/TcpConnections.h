#pragma once
#include "Connections.h"
#include <SFML/Network.hpp>

namespace Pong
{
	namespace Internet
	{
		class TcpConnection: public Pong::Internet::Connection
		{
			static int NextId;
			int myId;
			sf::TcpSocket socket;
			std::function<void(Connection*)> disconnectAction,
				errorAction;
			std::vector<byte> lastBuffer;
		public:
			TcpConnection(std::string ip, int port)
			{
				sf::Socket::Status status = socket.connect(ip, port);
				if (status != sf::Socket::Done)
					throw std::exception("Connection error");
				myId = NextId++;
			}

			std::vector<byte> ReadAllBytes() override
			{
				std::vector<byte> received = lastBuffer;
				lastBuffer.clear();
				const int bufferSize = 100;
				byte buffer[bufferSize];
				std::size_t count;
				socket.setBlocking(false);

				sf::Socket::Status status;
				do
				{
					status=socket.receive(buffer, bufferSize, count);
					DoErrorActionIfNeeded(status);
					for (auto i = 0; i < count/sizeof(byte); i++)
						received.push_back(buffer[i]);
				} while (count>=bufferSize || status==sf::Socket::NotReady);
				return received;
			}

		private:
			void DoErrorActionIfNeeded(sf::TcpSocket::Status status)
			{
				if (status != sf::Socket::Status::Done && status!=sf::Socket::NotReady)
				{
					if (status == sf::Socket::Disconnected)
						if (disconnectAction != nullptr)
							disconnectAction(this);
					if (status == sf::Socket::Error)
						if (errorAction != nullptr)
							errorAction(this);
					throw std::exception("Input/ouput socket error");
				}
			}

		public:
			std::vector<byte> ReadBytes(unsigned size) override
			{
				auto received = lastBuffer;
				lastBuffer.clear();
				const int bufferSize = 100;
				byte buffer[bufferSize];
				std::size_t count;
				socket.setBlocking(true);

				while(received.size()<size)
				{
					auto status = socket.receive(buffer, bufferSize, count);
					DoErrorActionIfNeeded(status);
					for (auto i = 0; i < count / sizeof(byte); i++)
						received.push_back(buffer[i]);
				}

				return CutAdditionalBytes(size, received);
			}

		private:
			std::vector<byte>& CutAdditionalBytes(unsigned size, std::vector<byte> &received)
			{
				for (int i = size; i < received.size(); i++)
					lastBuffer.push_back(received[i]);
				while(received.size()>size)
					received.pop_back();
				return received;
			}

		public:
			std::vector<byte> ReadBytesToDelimiter(byte delimiter) override
			{
				auto received = lastBuffer;
				lastBuffer.clear();
				const int bufferSize = 100;
				byte buffer[bufferSize];
				std::size_t count;
				unsigned size = 0;
				socket.setBlocking(true);

				for (int i = 0; i<received.size(); i++)
				{
					if (received[i] == delimiter) {
						size = i + 1;
						break;
					}
				}

				while (size==0)
				{
					auto status = socket.receive(buffer, bufferSize, count);
					DoErrorActionIfNeeded(status);
					for (auto i = 0; i < count / sizeof(byte); i++) {
						received.push_back(buffer[i]);
						if (buffer[i] == delimiter) size = received.size();
					}
				}

				return CutAdditionalBytes(size, received);
			}

			void SendBytes(std::vector<byte> bytes) override
			{
				socket.setBlocking(true);
				auto status = socket.send(bytes.data(), bytes.size());
				DoErrorActionIfNeeded(status);
			}

			virtual unsigned GetId()
			{
				return myId;
			}

			virtual void SetDisconnectAction(std::function<void(Connection*)> func)
			{
				disconnectAction = func;
			}

			virtual void SetTransmissionErrorAction(std::function<void(Connection*)> func)
			{
				errorAction = func;
			}
		};
	}
}