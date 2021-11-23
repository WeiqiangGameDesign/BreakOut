#include "Paddle.h"
#include <iostream>


Paddle::Paddle(float width, float height, sf::Vector2f pos)
{
	sf::RectangleShape tmp(sf::Vector2f(width, height));
	paddleShape = tmp;
	paddleShape.setPosition(pos);
}

Paddle::~Paddle()
{

}

void Paddle::setFillColor(sf::Color color)
{
	paddleShape.setFillColor(color);
}

void Paddle::move(float x, float y)
{
	paddleShape.move(x, y);
	//std::cout << "moving" << std::endl;
	//std::cout << x << std::endl;
}

void Paddle::setBoundary(float leftB, float rightB)
{
	leftBoundary = leftB;
	rightBoundary = rightB;
}

float Paddle::getMidPoint()
{
	float res = paddleShape.getPosition().x + paddleShape.getSize().x / 2.f;
	return res;
}


bool Paddle::boundaryCheckLeft()
{
	if (paddleShape.getPosition().x < leftBoundary) {
		return true;
	}
	return false;
}

bool Paddle::boundaryCheckRight() {
	if (paddleShape.getPosition().x + paddleShape.getSize().x > rightBoundary) {
		return true;
	}
	return false;
}
