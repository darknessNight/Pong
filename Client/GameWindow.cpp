#include "GameWindow.h"
#include <Windows.h>

GameWindow::GameWindow(std::string adres, int port) {
	this->adres = adres;
	this->port = port;
}


bool GameWindow::IsWindowOpen()
{
	std::lock_guard<std::mutex> lock(mutex);
	return window.isOpen();
}

bool GameWindow::PoolEventIfCan(sf::Event &event)
{
	std::lock_guard<std::mutex> lock(mutex);
	return window.isOpen() && window.pollEvent(event);
}

void GameWindow::start() {

	try {
		Pong::Internet::JsonClientProtocolConnectionFactory factory;
		connection = factory.GetConnectionFor(adres, port);
	}
	catch(std::exception e)
	{
		//TODO nie mo¿na by³o siê po³¹czyæ. WYJATEK CHUJU!
	}

	this->window.create(sf::VideoMode(WINDOWSIZE, WINDOWSIZE), "PONG", sf::Style::Close);
	this->stopDrawing = false;

	this->window.setActive(false);
	sf::Thread drawingThread(&GameWindow::startDrawing, this);
	drawingThread.launch();

	std::map<sf::Keyboard::Key, bool> pressed;
	pressed[sf::Keyboard::Up] = false;
	pressed[sf::Keyboard::Down] = false;
	pressed[sf::Keyboard::Left] = false;
	pressed[sf::Keyboard::Right] = false;
	pressed[sf::Keyboard::Space] = false;
	pressed[sf::Keyboard::Z] = false;

	while (IsWindowOpen())
	{

		sf::Event event;

		while (PoolEventIfCan(event))
		{

			if (event.type == sf::Event::Closed) {
				this->stopDrawing = true;
				window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up) {
					if (!pressed[sf::Keyboard::Up]) {
						connection->SendActionToServer(Pong::Internet::UserActionTypes::Shoot);
						pressed[sf::Keyboard::Up] = true;
					}
				}
				else if (event.key.code == sf::Keyboard::Down) {
				}
				else if (event.key.code == sf::Keyboard::Left) {
					if (!pressed[sf::Keyboard::Left] && !pressed[sf::Keyboard::Right]) {
						connection->SendActionToServer(Pong::Internet::UserActionTypes::StartMoveLeft);
						pressed[sf::Keyboard::Left] = true;
					}
				}
				else if (event.key.code == sf::Keyboard::Right) {
					if (!pressed[sf::Keyboard::Left] && !pressed[sf::Keyboard::Right]) {
						connection->SendActionToServer(Pong::Internet::UserActionTypes::StartMoveRight);
						pressed[sf::Keyboard::Right] = true;
					}
				}
				else if (event.key.code == sf::Keyboard::Space) {
					if (!pressed[sf::Keyboard::Space]) {
						connection->SendActionToServer(Pong::Internet::UserActionTypes::Shield);
						pressed[sf::Keyboard::Space] = true;
					}
				}
				else if (event.key.code == sf::Keyboard::Z) {

				}
			}

			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Up) {
					if (pressed[sf::Keyboard::Up]) {
						pressed[sf::Keyboard::Up] = false;
					}
				}
				else if (event.key.code == sf::Keyboard::Space) {
					if (pressed[sf::Keyboard::Space]) {
						pressed[sf::Keyboard::Space] = false;
					}
				}
				else if (event.key.code == sf::Keyboard::Left) {
					if (pressed[sf::Keyboard::Left]) {
						connection->SendActionToServer(Pong::Internet::UserActionTypes::StopMoveLeft);
						pressed[sf::Keyboard::Left] = false;
					}
				}
				else if (event.key.code == sf::Keyboard::Right) {
					if (pressed[sf::Keyboard::Right]) {
						connection->SendActionToServer(Pong::Internet::UserActionTypes::StopMoveRight);
						pressed[sf::Keyboard::Right] = false;
					}
				}
			}
		}
	}

}


void GameWindow::startDrawing() {
	while (!this->stopDrawing) {
		std::vector<Pong::Internet::ConnectionObject> temp;
		try {
			temp = connection->GetLatestObjectsFromServer();
			//TODO on nie czeka, a¿ bêdzie coœ, tylko zwraca pust¹ tablicê, zrób z tym co chcesz
			while (temp.size() == 0) {
				temp = connection->GetLatestObjectsFromServer();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
		catch (std::exception e)
		{
			::ShowWindow(::GetConsoleWindow(), SW_HIDE);
			std::cout << "Error: " << e.what();
			MessageBoxA(nullptr, e.what(), "Error", 0);
			std::lock_guard<std::mutex> lock(mutex);
			window.close();
			return;
		}
		//---------------------------------------------------------

		//TODO tutaj przyda³oby siê na³o¿yæ mutexy na to okno, ale póki siê nie wywala przez to, to spoko

		this->window.clear(sf::Color::Black);

		sf::RectangleShape wall(sf::Vector2f(WINDOWSIZE, WINDOWSIZE*Pong::Consts::BALL_RADIUS));
		wall.setPosition(sf::Vector2f(0, 0));
		wall.setFillColor(sf::Color::Yellow);
		this->window.draw(wall);



		/*for (auto el : temp)
		{
			std::cout << "Object: " << el.type << " pos: " << el.x << " " << el.y << "\n";
		}*/

		for (int i = 0; i < temp.size(); i++) {

			if (temp[i].type == Pong::Internet::ConnectionObject::BallCommon) {
				sf::CircleShape circle(Pong::Consts::BALL_RADIUS*WINDOWSIZE);
				circle.setPosition(sf::Vector2f(temp[i].x*WINDOWSIZE, temp[i].y*WINDOWSIZE));
				circle.setFillColor(sf::Color::Green);
				this->window.draw(circle);
				std::cout << "Pos: " << temp[i].x << " " << temp[i].y << "\n";
			}
			if (temp[i].type == Pong::Internet::ConnectionObject::BallRed) {
				sf::CircleShape circle(Pong::Consts::BALL_RADIUS*WINDOWSIZE);
				circle.setPosition(sf::Vector2f(temp[i].x*WINDOWSIZE, temp[i].y*WINDOWSIZE));
				circle.setFillColor(sf::Color::Red);
				this->window.draw(circle);
			}
			if (temp[i].type == Pong::Internet::ConnectionObject::Player3) {
				if (temp[i].lives > 0) {
					sf::RectangleShape player1Rect(sf::Vector2f(WINDOWSIZE*Pong::Consts::PLAYER_WIDTH, WINDOWSIZE*Pong::Consts::BALL_RADIUS));
					player1Rect.rotate(90);
					player1Rect.setPosition(sf::Vector2f((temp[i].x + Pong::Consts::BALL_RADIUS)*WINDOWSIZE , temp[i].y*WINDOWSIZE));
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
					wall.setPosition(sf::Vector2f(WINDOWSIZE*Pong::Consts::BALL_RADIUS, 0));
					wall.setFillColor(sf::Color::Yellow);
					this->window.draw(wall);
				}
			}
			if (temp[i].type == Pong::Internet::ConnectionObject::Player2) {
				if (temp[i].lives > 0) {
					sf::RectangleShape player1Rect(sf::Vector2f(WINDOWSIZE*Pong::Consts::PLAYER_WIDTH, WINDOWSIZE*Pong::Consts::BALL_RADIUS));
					std::cout << "Player2 pos: " << temp[i].x << " " << temp[i].y << "\n";
					player1Rect.setPosition(sf::Vector2f((temp[i].x + Pong::Consts::BALL_RADIUS)*WINDOWSIZE, temp[i].y*WINDOWSIZE));
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
			if (temp[i].type == Pong::Internet::ConnectionObject::Player1) {
				if (temp[i].lives > 0) {
					sf::RectangleShape player1Rect(sf::Vector2f(WINDOWSIZE*Pong::Consts::PLAYER_WIDTH, WINDOWSIZE*Pong::Consts::BALL_RADIUS));
					player1Rect.rotate(90);
					player1Rect.setPosition(sf::Vector2f((temp[i].x + Pong::Consts::BALL_RADIUS)*WINDOWSIZE, temp[i].y*WINDOWSIZE));
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