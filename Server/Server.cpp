// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Hypodermic\ContainerBuilder.h>
#include "../ServerLibrary/IoC.h"
#include "../ServerLibrary/InternetProtocol/TcpSocketConnections.h"

int main()
{
	Hypodermic::ContainerBuilder builder;
	builder.registerType<Pong::Internet::TcpSocketServer>().as<Pong::Internet::Server>();
	
	IoC::SetContainer(builder.build());

	auto server = IoC::GetContainer()->resolve<Pong::Internet::Server>();
	server->StartListening();

    return 0;
}

