/*Checking SFML
*/
#pragma once
#include <SFML\Graphics.hpp>
#include "Tests.h"

bool Test1();
AutoAdd AA1(&Test1, "IOModule", "SFML_Test",true);

bool Test1() {
	sf::RenderWindow okno(sf::VideoMode(800, 600), "Test1");
	sf::Clock stoper;
	sf::CircleShape ks1(200,4), ks2(100, 8), ks3(200, 16);
	ks1.setPosition(50, 50);
	ks2.setPosition(500, 300);
	ks3.setPosition(200, 50);
	ks1.setFillColor(sf::Color::Red);
	ks2.setFillColor(sf::Color::Green);
	ks3.setFillColor(sf::Color::Blue);
	okno.setVerticalSyncEnabled(true);
	sf::Texture text1;
	text1.create(100, 100);
	text1.loadFromFile("./Tests/Resources/Image.png");
	sf::Sprite sp1,sp2;
	sp1.setTexture(text1);
	sp2.setTexture(text1);
	sp2.setScale({ 2,2 });
	sp2.setPosition(300, 200);
	while (okno.isOpen())
	{
		sf::Event event;
		while (okno.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				okno.close();

		}
		okno.clear();

		sf::CircleShape ksztalt(std::sin(stoper.getElapsedTime().asSeconds()) * okno.getSize().y / 8 + okno.getSize().y / 4);
		ksztalt.setOrigin(sf::Vector2f(ksztalt.getRadius(), ksztalt.getRadius()));
		ksztalt.setPosition(okno.getSize().x / 2.0f, okno.getSize().y / 2.0f);
		ksztalt.setFillColor(sf::Color::Yellow);
		okno.draw(ks1);
		okno.draw(ks2);
		okno.draw(ks3);
		okno.draw(ksztalt);
		okno.draw(sp1);
		okno.draw(sp2);
		sp2.setPosition(std::sin(stoper.getElapsedTime().asSeconds()) * 100 + 300, std::cos(stoper.getElapsedTime().asSeconds()) * 100 + 200);

		okno.display();
	}
	return PromptMessage("Czy widziales okno z ksztaltami i pulsujacym kolkiem?");
}