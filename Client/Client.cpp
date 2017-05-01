// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//#include <Hypodermic\ContainerBuilder.h>
#include "../ServerLibrary/InternetProtocol/TcpConnectionFactory.h"
//#include "../ServerLibrary/IoC.h"

int main()
{
	/*Hypodermic::ContainerBuilder builder;
	builder.registerType<Pong::Internet::TcpConnectionFactory>().as<Pong::Internet::ConnectionFactory>();
	//other dependencies

	IoC::SetContainer(builder.build());

	//sample
	auto connectionFactory = IoC::GetContainer()->resolve<Pong::Internet::ConnectionFactory>();*/

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}
	return 0;
}

