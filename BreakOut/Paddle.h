#pragma once
#include<SFML/Graphics.hpp>


class Paddle {

public:
	Paddle(float width, float height, sf::Vector2f pos);
	~Paddle();

	void setFillColor(sf::Color color);

	void move(float x, float y);

	void setBoundary(float x, float y);

	float getMidPoint();

	sf::RectangleShape paddleShape;


	bool boundaryCheckLeft();
	bool boundaryCheckRight();

private:
	float leftBoundary, rightBoundary;


};

