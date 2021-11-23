// BreakOut.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "CollisionDetectUtil.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "PointCounter.h"
#include "Menu.h"

enum States {
	MENU_STATE,
	PLAY,
	RESULT,
	QUIT
};

int main()
{
	int gameMode = MENU_STATE;
	// setup the window
	float winWidth = 2400.f;
	float winHeight = 1600.f;
	sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "Break Out!");
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);


	// set pop sound
	sf::SoundBuffer popBuffer;
	if (!popBuffer.loadFromFile("./audios/pop.ogg")) {
		std::cout << "fail to load pop.ogg" << std::endl;
		return -1;
	}

	sf::Sound popSound;
	popSound.setBuffer(popBuffer);

	sf::SoundBuffer brickBuffer;
	if (!brickBuffer.loadFromFile("./audios/beep.ogg")) {
		std::cout << "fail to load beep.ogg" << std::endl;
		return -1;
	}
	sf::Sound brickSound;
	brickSound.setBuffer(brickBuffer);

	sf::SoundBuffer powerBuffer;
	if (!powerBuffer.loadFromFile("./audios/Powerup.wav")) {
		std::cout << "fail to load power.wav" << std::endl;
		return -1;
	}

	sf::Sound powerUpSound;
	powerUpSound.setBuffer(powerBuffer);

	sf::SoundBuffer wallBuffer;
	if (!wallBuffer.loadFromFile("./audios/wall.ogg")) {
		std::cout << "fail to load wall.ogg" << std::endl;
		return -1;
	}

	sf::Sound wallSound;
	wallSound.setBuffer(wallBuffer);


	sf::SoundBuffer loseBuffer;
	if (!loseBuffer.loadFromFile("./audios/theLose.wav")) {
		std::cout << "fail to load lose.wav" << std::endl;
		return -1;
	}

	sf::Sound loseSound;
	loseSound.setBuffer(loseBuffer);

	// setup Clock
	sf::Clock clock;

	Menu menu(window.getSize().x, window.getSize().y);
	menu.isOpen = true;

	srand(time(NULL));

	PointCounter lifeCounter(sf::Vector2f(30.f, winHeight - 36.f));
	lifeCounter.setPoints(3, "life: ");
	PointCounter scoreCounter(sf::Vector2f(winWidth - 240.f, 36.f));
	scoreCounter.setPoints(0, "score: ");

	float ballSpeed = 6.f;
	float paddleSpeed = 480.f;

	Paddle paddle(360.f, 12.f, sf::Vector2f(winWidth / 2.f - 180.f, winHeight - 36.f));
	paddle.setFillColor(sf::Color::Yellow);
	paddle.setBoundary(0.f, winWidth);


	// setup ball texture
	sf::Texture ballTexture;
	//ballTexture.setSmooth(true);
	//ballTexture.setRepeated(true);
	ballTexture.loadFromFile("./images/suntexture.jpg");

	Ball ball(12.f, sf::Vector2f(paddle.getMidPoint() - 12.f, winHeight - 36.f - 24.f));
	ball.isLaunch = false;
	ball.setBoundary(0, winWidth, 0, winHeight);
	ball.ballShape.setTexture(&ballTexture);

	sf::Texture brickTexture;
	brickTexture.loadFromFile("./images/mybrick.jpg");

	sf::Color brickColor(sf::Color::White);
	std::vector<std::vector<Brick>> brickSheet;
	float brickWidth = 192.f, brickHeight = 72.f;
	for (int i = 0; i < 4; i++) {
		std::vector<Brick> brickLine;
		for (int j = 0; j < 8; j++) {
			Brick tmp(brickWidth, brickHeight, sf::Vector2f(brickWidth * (j + 2), (brickHeight * (i + 2))));
			//brickColor.r = rand() % 255;
			tmp.brickShape.setFillColor(brickColor);
			tmp.brickShape.setTexture(&brickTexture);
			tmp.isDestoryed = false;
			brickLine.push_back(tmp);
		}
		brickSheet.push_back(brickLine);
	}

	// setup result
	sf::Text resText;
	sf::Font resFont;
	if (!resFont.loadFromFile("./fonts/open-sans/OpenSans-Light.ttf")) {
		std::cout << "resfont load error" << std::endl;
	}
	resText.setFont(resFont);
	resText.setPosition(winWidth / 2 - 360.f, winHeight / 2);


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (gameMode == MENU_STATE) {
					switch (event.key.code) {
					case sf::Keyboard::Up:
					case sf::Keyboard::W:
						menu.MoveUp();
						break;
					case sf::Keyboard::Down:
					case sf::Keyboard::S:
						menu.MoveDown();
						break;
					case sf::Keyboard::Return:
					//case sf::Keyboard::Space:
						switch (menu.getPressedItem())
						{
						case 0:
							std::cout << "Play" << std::endl;
							menu.isOpen = false;
							//lastMode = MENU_STATE;
							gameMode = PLAY;
							break;
						case 1:
							std::cout << "Exit" << std::endl;
							//closeMenu = true;
							//lastMode = MENU_STATE;
							gameMode = QUIT;
							window.close();
							break;
						}

					}
				}
			}

		}
		// window scale set up
		sf::View view;
		auto windowSize = window.getSize();
		view.reset(sf::FloatRect(0.f, 0.f, windowSize.x, windowSize.y));
		window.setView(view);


		if (gameMode == PLAY) {
			float delta_s = clock.getElapsedTime().asSeconds();
			clock.restart();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (!paddle.boundaryCheckLeft()) {
					auto position = paddle.paddleShape.getPosition();
					float deltaX = paddleSpeed * delta_s * -1.f;
					//std::cout << deltaX << std::endl;
					paddle.move(deltaX, 0);
				}
				if (ball.isLaunch == false) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						ball.speed = ballSpeed;
						ball.dir = sf::Vector2f(-1.f, -1.f);
						ball.launch(ball.dir, ball.speed);
						ball.isLaunch = true;
						popSound.play();
					}
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				if (!paddle.boundaryCheckRight()) {
					auto position = paddle.paddleShape.getPosition();
					float deltaX = paddleSpeed * delta_s * 1.f;
					//std::cout << deltaX << std::endl;
					paddle.move(deltaX, 0);
				}
				if (ball.isLaunch == false) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
						ball.speed = ballSpeed;
						ball.dir = sf::Vector2f(1.f, -1.f);
						ball.launch(ball.dir, ball.speed);
						ball.isLaunch = true;
						popSound.play();
					}
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if (ball.isLaunch == false) {
					ball.speed = ballSpeed;
					ball.dir = sf::Vector2f(0.f, -1.f);
					ball.launch(ball.dir, ball.speed);
					ball.isLaunch = true;
					popSound.play();
					//std::cout << "ball is Launch" << ball.isLaunch << std::endl;
				}
			}

			ball.run();
			bool isCollidedWithPaddle = CollisionDetectUtil::isBoxCirlcleIntersected(paddle.paddleShape, ball.ballShape);
			if (isCollidedWithPaddle) {
				if (ball.isLaunch != false) {
					popSound.play();
				}
				int deltaVelX = ball.ballShape.getPosition().x - paddle.getMidPoint();
				ball.dir.y = -1;
				ball.reflect(sf::Vector2f(deltaVelX * 0.01f, 0.f));
				isCollidedWithPaddle = false;
			}
			else {

				if (ball.leftBoundaryCheck()) {
					ball.dir.x *= -1;
					ball.reflect(sf::Vector2f(0.f, 0.f));
					if(ball.isLaunch)
						wallSound.play();
				}
				else if (ball.rightBoundaryCheck()) {
					ball.dir.x *= -1;
					ball.reflect(sf::Vector2f(0.f, 0.f));
					if(ball.isLaunch)
					wallSound.play();
				}
				if (ball.topBoundaryCheck()) {
					ball.dir.y *= -1;
					ball.reflect(sf::Vector2f(0.f, 0.f));
					if(ball.isLaunch)
					wallSound.play();
				}
				else if (ball.bottomBoundaryCheck()) {
					if (ball.isLaunch == true) {
						bool isBottomTouched = ball.bottomBoundaryCheck();
						lifeCounter.MinusPoint(1, "life: ");
						ball.isLaunch = false;
						ball.reset(0.f, sf::Vector2f(0.f, 0.f), sf::Vector2f(paddle.getMidPoint(), winHeight - 36.f - 24.f));
						loseSound.play();
						if (lifeCounter.getPoints() == 0) {
							gameMode = RESULT;
						}
					}
				}
			}

			int breakCount = 0;
			for (int i = 0; i < brickSheet.size(); i++) {
				for (int j = 0; j < brickSheet[i].size(); j++) {
					if (brickSheet[i][j].brickShape.getFillColor() != sf::Color::Black) {
						breakCount++;
					}
					bool isColidedWithBrick = CollisionDetectUtil::isBoxCirlcleIntersected(brickSheet[i][j].brickShape, ball.ballShape);
					if (isColidedWithBrick && !brickSheet[i][j].isDestoryed) {
						sf::Vector2f tmp = CollisionDetectUtil::recoilVel(brickSheet[i][j].getCenter(), ball.getCenter());
						if (abs(tmp.x) > brickSheet[i][j].brickShape.getSize().x / 2.f) {
							ball.dir.x *= -1;
						}
						if (abs(tmp.y) > brickSheet[i][j].brickShape.getSize().y / 2.f) {
							ball.dir.y *= -1;
						}
						brickSheet[i][j].isDestoryed = true;
						brickSound.play();
						brickSheet[i][j].brickShape.setFillColor(sf::Color::Black);
						scoreCounter.AddPoint(100, "score: ");
					}
				}
			}

			if (breakCount == 0) {
				for (int i = 0; i < brickSheet.size(); i++) {
					for (int j = 0; j < brickSheet[i].size(); j++) {
						brickSheet[i][j].isDestoryed = false;
						brickSheet[i][j].brickShape.setFillColor(sf::Color::White);
					}
				}
				ballSpeed += 6.f;
				ball.speed = ballSpeed;
				powerUpSound.play();
			}


			if (ball.isLaunch == false) {
				ball.ballShape.setPosition(sf::Vector2f(paddle.getMidPoint() - 12.f, paddle.paddleShape.getPosition().y - 24.f));
			}


			window.clear();
			for (int i = 0; i < brickSheet.size(); i++) {
				for (int j = 0; j < brickSheet[i].size(); j++) {
					window.draw(brickSheet[i][j].brickShape);
				}
			}
			lifeCounter.draw(window);
			scoreCounter.draw(window);
			window.draw(paddle.paddleShape);
			window.draw(ball.ballShape);
		}
		else if (gameMode == MENU_STATE) {
			window.clear();
			menu.draw(window, !menu.isOpen);
		}
		else if (gameMode == RESULT) {
			window.clear();
			if (lifeCounter.getPoints() == 0) {

				resText.setString("Your Final Score:" + std::to_string(scoreCounter.getPoints()) + "(Press Enter Start Again)");
				resText.setCharacterSize(50.f);
				window.draw(resText);
			}
			/*
			else{
				resText.setString("WON! Press Space Play again in faster speed");
				resText.setCharacterSize(50.f);
				window.draw(resText);
			}
			*/

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				gameMode = PLAY;
				ball.reset(0.f, sf::Vector2f(0.f, 0.f), sf::Vector2f(paddle.getMidPoint(), winHeight - 36.f - 24.f));
				lifeCounter.restart(3, "life: ");
				scoreCounter.restart(0, "score: ");
				ball.isLaunch = false;
				ballSpeed = 6.f;
				for (int i = 0; i < brickSheet.size(); i++) {
					for (int j = 0; j < brickSheet[i].size(); j++) {
						brickSheet[i][j].isDestoryed = false;
						brickSheet[i][j].brickShape.setFillColor(sf::Color::White);
					}
				}
				//powerUpSound.play();
			}

		}

		window.display();
	}


}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
