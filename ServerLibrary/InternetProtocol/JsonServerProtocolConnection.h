#pragma once
#include <map>
#include "ProtocolConnections.h"
#include <json.hpp>

using json = nlohmann::json;

namespace Pong
{
	namespace Internet
	{
		class JsonServerProtocolConnection : public ServerProtocolConnection
		{
			std::map<unsigned, std::shared_ptr<Connection>> users;
			std::map<unsigned, unsigned> usersIds;
		public:
			JsonServerProtocolConnection()
			{

			}

		private:
			void SendIdToUser(std::shared_ptr<Connection> user)
			{
				json message;
				message["playerId"] = usersIds[user->GetId()];
				auto data = message.dump();
				std::vector<unsigned char> bytes(data.begin(), data.end());
				bytes.push_back(0);
				user->SendBytes(bytes);
			}

		public:
			unsigned AddUserConnectionAndGetId(std::shared_ptr<Connection> user) override
			{
				users[user->GetId()] = user;
				if (usersIds.find(user->GetId()) == usersIds.end())
					usersIds[user->GetId()] = users.size() - 1;

				SendIdToUser(user);

				return user->GetId();
			};

			void SendObjectsToAll(std::vector<ConnectionObject> objects) override
			{
				for (auto user : users)
					SendObjectsToUser(objects, user.second);
			};

			void SendObjectsToUser(const std::vector<ConnectionObject>& objects, std::shared_ptr<Connection> user) override
			{
				json message;
				for (auto obj : objects)
					message.push_back(json{
						{"x",obj.x},
						{"y", obj.y},
						{"type", static_cast<int>(obj.type)},
						{"lives", obj.lives},
						{"shielded",obj.shielded}
				});
				auto data = message.dump();

				std::vector<unsigned char> bytes(data.begin(), data.end());
				bytes.push_back(0);
				user->SendBytes(bytes);
			}

			std::vector<UserMove> GetActionsForUser(std::shared_ptr<Connection> user) override
			{
				std::vector<UserMove> result;
				while (user->IsSomethingToReceive()) {
					auto data = user->ReadBytesToDelimiter(0);
					std::string message = reinterpret_cast<char*>(data.data());

					json object = json::parse(message.begin(), message.end());

					UserMove move;
					move.moveType = object["move"];
					move.time = std::chrono::microseconds(object["time"]);
					result.push_back(move);
				}
				return result;
			}

			std::vector<UserMove> GetActionsForUserId(unsigned id) override
			{
				return GetActionsForUser(users[id]);
			}

			virtual std::shared_ptr<Connection> GetUserForId(unsigned id)
			{
				return users[id];
			}
		};
	}
}
