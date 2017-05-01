// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <Hypodermic/ContainerBuilder.h>
//#include "../ServerLibrary/IoC.h"
#include "../ServerLibrary/InternetProtocol/TcpConnection.h"
#include <SFML/Network.hpp>
#include <thread>
#include <iostream>
#include "ServerLibrary/InternetProtocol/TcpServer.h"

int main()
{
	/*Hypodermic::ContainerBuilder builder;
	builder.registerType<Pong::Internet::TcpSocketServer>().as<Pong::Internet::Server>();
	
	IoC::SetContainer(builder.build());

	auto server = IoC::GetContainer()->resolve<Pong::Internet::Server>();
	server->StartListening();*/

	
	bool accepted = false;
	

	Pong::Internet::TcpServer server;
	server.SetConnectAction([&](std::shared_ptr<Pong::Internet::Connection> connection)
	{
		accepted = true;
	});

	try {
		server.StartListeningAsync(53000);
	}
	catch(std::exception e)
	{
		std::cout << "Koniec swiata";
	}

	std::thread th([&]() {
		try {
			Pong::Internet::TcpConnection connection("127.0.0.1", 53000);
		}
		catch(...)
		{
			
		}
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	if (!accepted)
		std::cout << "porazka odbierania\n";
	else std::cout << "Sukces odbierania\n";

	if(th.joinable())
		th.join();

	std::cout << std::endl;
	system("pause");
    return 0;
}

