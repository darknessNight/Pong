#pragma once
#include "InternetProtocol/TcpServer.h"
#include "InternetProtocol/JsonServerProtocolConnection.h"
#include "ProtocolConnectionView.h"
#include "GameEngine/GameEngine.h"
#include "InternetProtocol/JsonClientProtocolConnection.h"
#include "DeadWall.h"
#include "CommonBall.h"

namespace Pong
{
	class Pong
	{
		Internet::TcpServer server;
		std::shared_ptr<Internet::ServerProtocolConnection> protocolServer;
		std::shared_ptr<GameEngine::View> protocolView;
		std::shared_ptr<GameEngine::GameEngine> gameEngine;
		std::vector<std::shared_ptr<PlayerObject>> players;
		bool working = true;
	public:
		void StartServer(int port)
		{
			server.SetConnectAction([&](std::shared_ptr<Internet::Connection> conn) {AcceptConnection(conn); });
			server.StartListeningAsync(port);

			protocolServer = std::make_shared<Internet::JsonServerProtocolConnection>();
			protocolView = std::make_shared<ProtocolConnectionView>(protocolServer);
			gameEngine = std::make_shared<GameEngine::GameEngine>();

			while (working)
			{
				if (players.size() >= 3)
				{
					StartGame();
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			while (working)
			{
				std::cout << "Przyjmuje komendy\n";
				std::string command;
				std::cin >> command;
				if (command == "stop")
					break;
			}
			gameEngine->Stop();
			server.StopListening();
		}
	private:
		void AcceptConnection(std::shared_ptr<Internet::Connection> connection)
		{
			auto id = protocolServer->AddUserConnectionAndGetId(connection);
			auto user = std::make_shared<Internet::ServerUserPartConnection>(protocolServer, id);
			players.push_back(std::make_shared<PlayerObject>(user,
				static_cast<Internet::UserIds>(Internet::UserIds::Player1 + id),
				[&](std::shared_ptr<GameEngine::GameObject> obj) {gameEngine->AddObject(obj); },
				[&](std::shared_ptr<GameEngine::GameObject> obj) {gameEngine->RemoveObject(obj); }
			));
		}

		void StartGame()
		{
			for (auto el : players)
			{
				gameEngine->AddObject(el);
				gameEngine->AddObject(std::make_shared<DeadWall>(el));
			}
			gameEngine->AddObject(std::make_shared<CommonBall>());
			gameEngine->AddObject(std::make_shared<GameEngine::GameObject>(
				GameEngine::Pointf{ -BoardWidth/2 ,0 }, GameEngine::Pointf{ BoardWidth*2,Consts::BALL_RADIUS },
				GameEngine::GameObject::Type::Wall
				));

			gameEngine->Start(protocolView);
		}
	};
}
