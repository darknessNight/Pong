#include "stdafx.h"
#include "CppUnitTest.h"

#include "../ServerLibrary/InternetProtocol/TcpConnections.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ServerIntegrationTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			boost::asio::io_service serv;
			boost::asio::ip::tcp::acceptor acceptor(serv);
			acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), 1450));
			acceptor.listen();
			boost::asio::ip::tcp::socket sock(serv);
			boost::asio::ip::tcp::endpoint endpoint;
			acceptor.async_accept(sock, endpoint, [] (const boost::system::error_code&) {
			
			});
			Pong::Internet::TcpConnection connection("::1", 1450);
			
			
		}

	};
}