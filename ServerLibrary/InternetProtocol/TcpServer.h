#pragma once
#include "Connections.h"
#include <SFML/Network.hpp>
#include <thread>
#include "TcpConnection.h"

namespace Pong
{
	namespace Internet
	{
		class TcpServer:public Server
		{
			sf::TcpListener listener;
			bool isAsync = false;
			bool working = false;
			std::function<void(std::shared_ptr<Connection>)> acceptAction;
			std::shared_ptr<std::thread> listeningThread;
		public:
			~TcpServer()
			{
				TcpServer::StopListening();
			}

			void StartListeningAsync(int port) override{
				if (working)
					return;
				listeningThread = std::make_shared<std::thread>([&]() {StartListening(port); });
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				isAsync = true;
			}

			void StartListening(int port) override{
				if (working)
					return;
				isAsync = false;
				working = true;
				if (listener.listen(port) != sf::Socket::Done)
					throw std::exception("Server start error");
				
				listener.setBlocking(false);

				while (working) {
					auto client = std::shared_ptr<TcpConnection>(new TcpConnection);
					sf::Socket::Status status;
					while ((status = listener.accept(client->socket)) == sf::Socket::NotReady) {
						if (!working)
							return;
						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
					if(status!=sf::Socket::Done)
						throw std::exception("Client accept error");
					if (acceptAction != nullptr)
						acceptAction(client);
				}
			}

			void StopListening() override{
				working = false;
				if(listeningThread!=nullptr)
				{
					if (listeningThread->joinable())
						listeningThread->join();
					listeningThread = nullptr;
				}
			}

			bool IsAsync() override{
				return isAsync;
			}

			void SetConnectAction(std::function<void(std::shared_ptr<Connection>)> func) override{
				acceptAction = func;
			}
		};
	}
}
