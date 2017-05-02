#pragma once
#include "ConnectionFactory.h"
#include "TcpConnection.h"

namespace Pong
{
	namespace Internet
	{
		class TcpConnectionFactory: public ConnectionFactory
		{
		public:
			std::shared_ptr<Connection> GetConnectionFor(std::string ip, int port) override
			{
				return std::make_shared<TcpConnection>(ip, port);
			};
		};
	}
}