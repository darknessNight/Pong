#pragma once
#include "ProtocolConnections.h"
#include "TcpConnectionFactory.h"
#include <thread>
#include <json.hpp>

using json = nlohmann::json;

namespace Pong
{
	namespace Internet
	{
		class Clock
		{
		public:
			virtual std::chrono::microseconds GetNow()
			{
				return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
			}
		};

		class JsonClientProtocolConnection: public ClientProtocolConnection
		{
		protected:
			std::shared_ptr<Connection> connection;
			UserIds myId;
			std::thread myThread;
			bool working;
			std::vector<std::vector<ConnectionObject>> messages;
			std::shared_ptr<Clock> myClock = std::make_shared<Clock>();
		public:
			explicit JsonClientProtocolConnection(std::shared_ptr<Connection> connection):connection(connection),
				myThread([&]() {ReceivingLoop(); })
			{}

			~JsonClientProtocolConnection()
			{
				working = false;
				connection->Abort();
				if (myThread.joinable())
					myThread.join();
			}

			void SetClock(std::shared_ptr<Clock> clock)
			{
				myClock = clock;
			}
		protected:
			void ReceivingLoop()
			{
				while (working)
				{
					std::vector<unsigned char> result;
					try {
						result = connection->ReadBytesToDelimiter(0);
					}catch(...){}
					if(result.size()>0)
					{
						auto message = json::parse(result.begin(), result.end());
						if (message.find("playerId") != message.end())
							myId = (UserIds)((int)message["playerId"]+(int)UserIds::Player1);
						else
						{
							std::vector<ConnectionObject> objects;
							for(auto el:message)
							{
								ConnectionObject obj;
								obj.x = el["x"];
								obj.y = el["y"];
								obj.type = el["type"];
								obj.lives = el["lives"];
								objects.push_back(obj);
							}
							messages.push_back(objects);
						}
					}
				}
			}
		public:
			std::vector<std::vector<ConnectionObject>> GetAllObjectsFromServer() override{
				auto all = messages;
				messages.clear();
				return all;
			}

			std::vector<ConnectionObject> GetLatestObjectsFromServer() override{
				auto latest = messages.back();
				messages.clear();
				return latest;
			}

			void SendActionToServer(UserActionTypes action) override{
				json message;
				message["time"] = myClock->GetNow().count();
				message["move"] = action;

				auto data = message.dump();
				data.push_back(0);

				connection->SendBytes(std::vector<unsigned char>(data.begin(), data.end()));
			}

			UserIds GetMyId() override{
				return myId;
			}
		};

		class JsonClientProtocolConnectionFactory:public ClientProtocolConnectionFactory
		{
			TcpConnectionFactory factory;
		public:
			std::shared_ptr<ClientProtocolConnection> GetConnectionFor(std::string ip, int port) override{
				return std::make_shared<JsonClientProtocolConnection>(factory.GetConnectionFor(ip, port));
			}
		};
	}
}
