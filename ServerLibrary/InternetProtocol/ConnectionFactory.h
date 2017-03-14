#pragma once
#include "Connections.h"

namespace Pong
{
	namespace Internet 
	{
		class ConnectionFactory abstract
		{
		public:
			virtual std::shared_ptr<Connection> GetConnectionFor(std::string ip, int port) = 0;
		};
	}
}