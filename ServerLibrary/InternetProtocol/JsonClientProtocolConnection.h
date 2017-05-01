#pragma once
#include "ProtocolConnections.h"

namespace Pong
{
	namespace Internet
	{
		class JsonClientProtoclConnection: public ClientProtocolConnection
		{

		public:
			std::vector<ConnectionObject> GetAllObjectsFromServer() override;
			std::vector<ConnectionObject> GetLatestObjectsFromServer() override;
			void SendActionToServer(UserActionTypes action) override;
			UserIds GetMyId() override;
		};
	}
}