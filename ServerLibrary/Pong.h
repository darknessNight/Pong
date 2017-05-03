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
				if(command=="show")
				{
					auto objs=gameEngine->GetAllObjects();
					int i = 0;
					for(auto el:objs)
					{
						std::cout <<i++<< ". Object: " << TypeToString(el->GetType()) << " Pos " << el->GetPos().x << " " << el->GetPos().y
							<< " PosEnd " << el->GetPos().x + el->GetSize().x << " " << el->GetPos().y + el->GetSize().y
							<< " Lives: " << el->GetLives() << "\n";
					}
				}
				if(command=="reset")
				{
					gameEngine->Stop();
					StartGame();
				}
			}
			gameEngine->Stop();
			server.StopListening();
		}
	private:
		std::string TypeToString(GameEngine::GameObject::Type type)
		{
			switch(type)
			{
			case GameEngine::GameObject::Type::DeadWall:return "Sciana smierci";
			case GameEngine::GameObject::Type::BallCommon:return "Zwykla pilka";
			case GameEngine::GameObject::Type::BallRed:return "Pilka smierci";
			case GameEngine::GameObject::Type::Wall:return "Zwykla sciana";
			case GameEngine::GameObject::Type::Player3:return "Gracz3";
			case GameEngine::GameObject::Type::Player1:return "Gracz1";
			case GameEngine::GameObject::Type::Player2:return "Gracz2";
			default:return "Smierci i zniszczenie. Nieznany typ";
			}
		}

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
			gameEngine->Clear();
			for (auto el : players)
			{
				el->Reset();
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
