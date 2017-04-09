#pragma once
#include "Connections.h"
#include <boost\asio.hpp>
#include <boost\asio\ip\tcp.hpp>
#include <boost\array.hpp>

namespace Pong
{
	namespace Internet
	{
		class TcpConnection: public Pong::Internet::Connection
		{
		private:
			boost::asio::io_service ioService;
			boost::asio::ip::tcp::socket socket;
		public:
			TcpConnection(std::string ip, int port):socket(ioService)
			{
				socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port));
			}

			virtual std::vector<byte> ReadAllBytes()
			{
				/*boost::array<byte, 128> buffer;
				std::vector<byte> result;
				while ( socket.receive(buffer) )
				{
					for ( auto el : buffer )
						result.push_back(el);
				}
				return result;*/
				return std::vector<byte>();
			}

			virtual std::vector<byte> ReadBytesToDelimiter(byte delimiter)
			{
				return std::vector<byte>();
			}

			virtual void SendBytes(std::vector<byte> bytes)
			{
				//socket.send(bytes);
			}

			virtual unsigned GetId()
			{
				return 0;
			}

			virtual void SetDisconnectAction(std::function<void(Connection*)>)
			{

			}

			virtual void SetTransmissionErrorAction(std::function<void(Connection*)>)
			{

			}
		};
	}
}