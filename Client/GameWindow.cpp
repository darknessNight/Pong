#include "GameWindow.h"

GameWindow::GameWindow(std::string adres, int port) {
	this->adres = adres;
	this->port = port;
}


void GameWindow::start() {

	this->window.create(sf::VideoMode(WINDOWSIZE, WINDOWSIZE), "PONG",sf::Style::Close);
	this->stopDrawing = false;

	this->window.setActive(false);
	sf::Thread drawingThread(&GameWindow::startDrawing,this);
	drawingThread.launch();

	while (window.isOpen())
	{

		sf::Event event;

		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed) {
				this->stopDrawing = true;
				window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up) {
					
				}
				else if (event.key.code == sf::Keyboard::Down) {

				}
				else if (event.key.code == sf::Keyboard::Left) {

				}
				else if (event.key.code == sf::Keyboard::Right) {

				}
				else if (event.key.code == sf::Keyboard::Space) {

				}
				else if (event.key.code == sf::Keyboard::Z) {

				}
			}

			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Up) {

				}
				else if (event.key.code == sf::Keyboard::Down) {

				}
				else if (event.key.code == sf::Keyboard::Left) {

				}
				else if (event.key.code == sf::Keyboard::Right) {

				}
			}
		}
	}

}


void GameWindow::startDrawing() {
	std::vector<Pong::Internet::ConnectionObject> temp;

	Pong::Internet::ConnectionObject ball, ballRed, ballRed2, player1, player2, player3;
	int checkInt = 0, asdf = 0;

	


	while (!this->stopDrawing) {

		//------------------------------------------------------
		ball.type = Pong::Internet::ConnectionObject::BallCommon;
		ball.x = 0.5;
		ball.y = 0.5;

		ballRed.type = Pong::Internet::ConnectionObject::BallRed;
		ballRed.x = 0.1;
		ballRed.y = 0.35;

		ballRed2.type = Pong::Internet::ConnectionObject::BallRed;
		ballRed2.x = 0.9;
		ballRed2.y = 0.736;

		player1.type = Pong::Internet::ConnectionObject::Player1;
		player1.x = 0.9;
		player1.shielded = false;
		player1.lives = checkInt;
		asdf++;
		if (asdf > 1000) {
			checkInt++;
			asdf = 0;
			if (checkInt == 4)
				checkInt = 0;
		}

		player2.type = Pong::Internet::ConnectionObject::Player2;
		player2.x = 0.5;
		player2.lives = 0;
		player2.shielded = true;

		player3.type = Pong::Internet::ConnectionObject::Player3;
		player3.x = 0.36;
		player3.lives = 3;
		player3.shielded = false;

		temp.clear();
		temp.push_back(ball);
		temp.push_back(ballRed);
		temp.push_back(ballRed2);
		temp.push_back(player1);
		temp.push_back(player2);
		temp.push_back(player3);
		//---------------------------------------------------------

		this->window.clear(sf::Color::Black);

		sf::RectangleShape wall(sf::Vector2f(WINDOWSIZE, WINDOWSIZE*Pong::Consts::BALL_RADIUS));
		wall.setPosition(sf::Vector2f(0,0));
		wall.setFillColor(sf::Color::Yellow);
		this->window.draw(wall);

		for (int i = 0; i < temp.size(); i++) {
			if (temp[i].type == Pong::Internet::ConnectionObject::BallCommon) {
				sf::CircleShape circle(Pong::Consts::BALL_RADIUS*WINDOWSIZE);
				circle.setPosition(sf::Vector2f(temp[i].x*WINDOWSIZE, temp[i].y*WINDOWSIZE));
				circle.setFillColor(sf::Color::Green);
				this->window.draw(circle);
			}
			if (temp[i].type == Pong::Internet::ConnectionObject::BallRed) {
				sf::CircleShape circle(Pong::Consts::BALL_RADIUS*WINDOWSIZE);
				circle.setPosition(sf::Vector2f(temp[i].x*WINDOWSIZE, temp[i].y*WINDOWSIZE));
				circle.setFillColor(sf::Color::Red);
				this->window.draw(circle);
			}
			if (temp[i].type == Pong::Internet::ConnectionObject::Player1) {
				if (temp[i].lives > 0) {
					sf::RectangleShape player1Rect(sf::Vector2f(WINDOWSIZE*Pong::Consts::PLAYER_WIDTH, WINDOWSIZE*Pong::Consts::BALL_RADIUS));
					player1Rect.rotate(90);
					player1Rect.setPosition(sf::Vector2f(WINDOWSIZE*Pong::Consts::BALL_RADIUS, temp[i].x*WINDOWSIZE));
					if (temp[i].shielded == true) {
						player1Rect.setFillColor(sf::Color::Blue);
					}
					else {
						if(temp[i].lives==3)
							player1Rect.setFillColor(sf::Color::Green);
						if (temp[i].lives == 2)
							player1Rect.setFillColor(sf::Color::Magenta);
						if (temp[i].lives == 1)
							player1Rect.setFillColor(sf::Color::Red);
					}
					this->window.draw(player1Rect);
				}
				else {
					sf::RectangleShape wall(sf::Vector2f(WINDOWSIZE, WINDOWSIZE*Pong::Consts::BALL_RADIUS));
					wall.rotate(90);
					wall.setPosition(sf::Vector2f(WINDOWSIZE*Pong::Consts::BALL_RADIUS, 0));
					wall.setFillColor(sf::Color::Yellow);
					this->window.draw(wall);
				}
			}
			if (temp[i].type == Pong::Internet::ConnectionObject::Player2) {
				if (temp[i].lives > 0) {
					sf::RectangleShape player1Rect(sf::Vector2f(WINDOWSIZE*Pong::Consts::PLAYER_WIDTH, WINDOWSIZE*Pong::Consts::BALL_RADIUS));
					player1Rect.setPosition(sf::Vector2f(temp[i].x*WINDOWSIZE, WINDOWSIZE - WINDOWSIZE*Pong::Consts::BALL_RADIUS));
					if (temp[i].shielded == true) {
						player1Rect.setFillColor(sf::Color::Blue);
					}
					else {
						if (temp[i].lives == 3)
							player1Rect.setFillColor(sf::Color::Green);
						if (temp[i].lives == 2)
							player1Rect.setFillColor(sf::Color::Magenta);
						if (temp[i].lives == 1)
							player1Rect.setFillColor(sf::Color::Red);
					}
					this->window.draw(player1Rect);
				}
				else {
					sf::RectangleShape wall(sf::Vector2f(WINDOWSIZE, WINDOWSIZE*Pong::Consts::BALL_RADIUS));
					wall.setPosition(sf::Vector2f(0, WINDOWSIZE - WINDOWSIZE*Pong::Consts::BALL_RADIUS));
					wall.setFillColor(sf::Color::Yellow);
					this->window.draw(wall);
				}
			}
			if (temp[i].type == Pong::Internet::ConnectionObject::Player3) {
				if (temp[i].lives > 0) {
					sf::RectangleShape player1Rect(sf::Vector2f(WINDOWSIZE*Pong::Consts::PLAYER_WIDTH, WINDOWSIZE*Pong::Consts::BALL_RADIUS));
					player1Rect.rotate(90);
					player1Rect.setPosition(sf::Vector2f(WINDOWSIZE, temp[i].x*WINDOWSIZE));
					if (temp[i].shielded == true) {
						player1Rect.setFillColor(sf::Color::Blue);
					}
					else {
						if (temp[i].lives == 3)
							player1Rect.setFillColor(sf::Color::Green);
						if (temp[i].lives == 2)
							player1Rect.setFillColor(sf::Color::Magenta);
						if (temp[i].lives == 1)
							player1Rect.setFillColor(sf::Color::Red);
					}
					this->window.draw(player1Rect);
				}
				else {
					sf::RectangleShape wall(sf::Vector2f(WINDOWSIZE, WINDOWSIZE*Pong::Consts::BALL_RADIUS));
					wall.rotate(90);
					wall.setPosition(sf::Vector2f(WINDOWSIZE, 0));
					wall.setFillColor(sf::Color::Yellow);
					this->window.draw(wall);
				}
			}

		}

		this->window.display();

	}
}