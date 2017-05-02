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
			virtual void SendObjectsToUser(const std::vector<ConnectionObject>& objects, std::shared_ptr<Connection>) = 0;
			virtual std::vector<UserMove> GetActionsForUser(std::shared_ptr<Connection>) =0;
			virtual std::vector<UserMove> GetActionsForUserId(unsigned id) = 0;
		};

		class ServerUserPartConnection
		{
			std::shared_ptr<ServerProtocolConnection> connection;
			unsigned userId;
		public:
			ServerUserPartConnection(std::shared_ptr<ServerProtocolConnection> conn, unsigned userId) :connection(conn)
			{}

			std::vector<UserMove> GetActions() const
			{
				return connection->GetActionsForUserId(userId);
			}
		};

		class ClientProtocolConnection abstract
		{
		public:
			virtual std::vector<std::vector<ConnectionObject>> GetAllObjectsFromServer() = 0;
			virtual std::vector<ConnectionObject> GetLatestObjectsFromServer() = 0;
			virtual void SendActionToServer(UserActionTypes action) = 0;
			virtual UserIds GetMyId() = 0;
		};

		class ClientProtocolConnectionFactory
		{
		public:
			virtual std::shared_ptr<ClientProtocolConnection> GetConnectionFor(std::string ip, int port) = 0;
		};
	}
}