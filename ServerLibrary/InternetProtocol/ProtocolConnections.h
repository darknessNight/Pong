#pragma once
#include "ConstantsAndStructs.h"
#include "Connections.h"

namespace Pong
{
	namespace Internet
	{
		class ServerProtocolConnection abstract
		{
		public:
			virtual unsigned AddUserConnectionAndGetId(std::shared_ptr<Connection>)=0;
			virtual void SendObjectsToAll(std::vector<ConnectionObject> objects)=0;
			virtual void SendObjectsToUser(std::vector<ConnectionObject> objects, std::shared_ptr<Connection>) = 0;
			virtual std::vector<UserActionTypes> GetActionsForUser(std::shared_ptr<Connection>)=0;
			virtual std::vector<UserActionTypes> GetActionsForUserId(unsigned id) = 0;
			virtual std::vector<std::pair<unsigned, std::vector<UserActionTypes>>> GetAllUsersActions() = 0;
		};

		class ClientProtocolConnection abstract
		{
		public:
			virtual std::vector<ConnectionObject> GetAllObjectsFromServer() = 0;
			virtual std::vector<ConnectionObject> GetLatestObjectsFromServer() = 0;
			virtual void SendActionToServer(UserActionTypes action) = 0;
			virtual UserIds GetMyId() = 0;
		};
	}
}