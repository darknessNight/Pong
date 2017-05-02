#pragma once
#include "InternetProtocol/JsonServerProtocolConnection.h"
#include "InternetProtocol/TcpServer.h"
#include "GameEngine/View.h"

namespace Pong
{
	class ProtocolConnectionView: public GameEngine::View
	{
		std::shared_ptr<Internet::ServerProtocolConnection> protocolServer;
	public:
		ProtocolConnectionView(std::shared_ptr<Internet::ServerProtocolConnection> server)
		{
			if (server == nullptr)
				throw std::exception("Server cannot be nullptr");
			protocolServer = server;
		}

		void DisplayObjects(const std::vector<std::shared_ptr<Pong::GameEngine::GameObject>>& objects) override{
			std::vector<Internet::ConnectionObject> connectionObjects;
			for(auto el:objects)
			{
				Internet::ConnectionObject obj;
				obj.lives = el->GetLives();
				obj.type = el->GetType();
				obj.x = el->GetPos().x;
				obj.y = el->GetPos().y;
				connectionObjects.push_back(obj);
			}

			protocolServer->SendObjectsToAll(connectionObjects);
		}
	};
}