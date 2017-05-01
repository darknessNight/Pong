// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <Hypodermic/ContainerBuilder.h>
//#include "../ServerLibrary/IoC.h"
#include "../ServerLibrary/InternetProtocol/TcpConnections.h"
#include <SFML/Network.hpp>
#include <thread>
#include <iostream>

int main()
{
	/*Hypodermic::ContainerBuilder builder;
	builder.registerType<Pong::Internet::TcpSocketServer>().as<Pong::Internet::Server>();
	
	IoC::SetContainer(builder.build());

	auto server = IoC::GetContainer()->resolve<Pong::Internet::Server>();
	server->StartListening();*/

	sf::TcpListener listener;

	// bind the listener to a port
	if (listener.listen(53000) != sf::Socket::Done)
	{
		// error...
	}

	// accept a new connection
	sf::TcpSocket client;
	std::vector<unsigned char> buffer;
	std::vector<unsigned char> data= { 1,2,3,4,5,6,7,8,9,0 };
	std::vector<unsigned char> expected = { 6,7,8,9,0 };
	std::thread th([&]() {
		if (listener.accept(client) != sf::Socket::Done)
		{
			throw std::exception();
		}
		std::size_t received;
		do
		client.send(data.data(), 10, received);
		while (received == 0);
	});

	try {
		Pong::Internet::TcpConnection connection("127.0.0.1", 53000);
		connection.ReadBytesToDelimiter(5);
		buffer = connection.ReadBytesToDelimiter(0);
	}
	catch(std::exception e)
	{
		std::cout << "Koniec swiata";
	}
	std::cout << "polaczone\n";

	if (buffer!=expected)
		std::cout << "porazka odbierania\n";
	else std::cout << "Sukces odbierania\n";

	if(th.joinable())
		th.join();

	std::cout << std::endl;
	system("pause");
    return 0;
}

