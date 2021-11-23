#pragma once

#include <SFML/Graphics.hpp>

class Ball {

public:
	Ball(float radius, sf::Vector2f pos);
	~Ball();
	sf::CircleShape ballShape;
	sf::Vector2f dir;
	float speed;
	bool isLaunch;

	void reflect(sf::Vector2f deltaVec);

	void reset(float speed, sf::Vector2f dir, sf::Vector2f pos);

	void launch(sf::Vector2f direction, float speed);

	void run();

	bool topBoundaryCheck();
	bool leftBoundaryCheck();
	bool rightBoundaryCheck();
	bool bottomBoundaryCheck();

	sf::Vector2f getCenter();

	void setBoundary(float left, float right, float top, float bottom);

private:

	float topBoundary;
	float leftBoundary;
	float rightBoundary;
	float bottomBoundary;

};
