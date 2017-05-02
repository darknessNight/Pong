#pragma once
#include "InternetProtocol/JsonServerProtocolConnection.h"
#include "InternetProtocol/TcpServer.h"
#include "GameEngine/View.h"
#include "PlayerObject.h"

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
				if(IsPlayer(el))
				{
					obj.shielded = static_cast<PlayerObject*>(el.get())->IsShielded();
				}
				obj.lives = el->GetLives();
				obj.type = el->GetType();
				obj.x = el->GetPos().x;
				obj.y = el->GetPos().y;
				connectionObjects.push_back(obj);
			}

			protocolServer->SendObjectsToAll(connectionObjects);
		}
	private:
		static bool IsPlayer(std::shared_ptr<Pong::GameEngine::GameObject> el)
		{
			return el->GetType() == Internet::ConnectionObject::Player1 ||
				el->GetType() == Internet::ConnectionObject::Player2 ||
				el->GetType() == Internet::ConnectionObject::Player3;
		}
	};
}
