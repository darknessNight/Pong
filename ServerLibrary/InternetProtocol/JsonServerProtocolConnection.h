#pragma once
#include <map>
#include "ProtocolConnections.h"
#include "../IoC.h"

namespace Pong
{
	namespace Internet
	{
		class JsonServerProtocolConnection: public ServerProtocolConnection
		{
			std::map<unsigned, std::shared_ptr<Connection>> users;
		public:
			JsonServerProtocolConnection()
			{

			}

			virtual unsigned AddUserConnectionAndGetId(std::shared_ptr<Connection>) = 0;
			virtual void SendObjectsToAll(std::vector<ConnectionObject> objects) = 0;
			virtual void SendObjectsToUser(std::vector<ConnectionObject> objects, std::shared_ptr<Connection>) = 0;
			virtual std::vector<UserActionTypes> GetActionsForUser(std::shared_ptr<Connection>) = 0;
			virtual std::vector<UserActionTypes> GetActionsForUserId(unsigned id) = 0;
			virtual std::vector<std::pair<unsigned, std::vector<UserActionTypes>>> GetAllUsersActions() = 0;
		};
	}
}
