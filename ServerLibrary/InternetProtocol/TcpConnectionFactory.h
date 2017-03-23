#pragma once
#include "ConnectionFactory.h"


namespace Pong
{
	namespace Internet
	{
		class TcpConnectionFactory: public ConnectionFactory
		{
		public:
			virtual std::shared_ptr<Connection> GetConnectionFor(std::string ip, int port) {
				return nullptr;
			};
		};
	}
}