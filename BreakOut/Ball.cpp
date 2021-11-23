#include "Ball.h"
#include <iostream>


Ball::Ball(float radius, sf::Vector2f pos)
{
	sf::CircleShape tmp(radius);
	ballShape = tmp;
	ballShape.setPosition(pos);
}

void Ball::reset(float speed, sf::Vector2f dir, sf::Vector2f pos)
{
	ballShape.setPosition(pos);
	this->dir = dir;
	this->speed = speed;
}

Ball::~Ball()
{
}

void Ball::reflect(sf::Vector2f deltaVec)
{
	dir += deltaVec;
	run();
}


void Ball::launch(sf::Vector2f direction, float ballSpeed)
{
	dir = direction;
	speed = ballSpeed;
}

void Ball::run()
{
	ballShape.move(dir * speed);
	std::cout << "ball y" << ballShape.getPosition().y << std::endl;
}

bool Ball::topBoundaryCheck()
{
	if (ballShape.getPosition().y < 0) {
		return true;
	}
	return false;
}

bool Ball::leftBoundaryCheck()
{
	if (ballShape.getPosition().x < 0) {
		return true;
	}
	return false;
}

bool Ball::rightBoundaryCheck()
{
	if (ballShape.getPosition().x + ballShape.getRadius() * 2.f > rightBoundary) {
		return true;
	}
	return false;
}

bool Ball::bottomBoundaryCheck()
{
	/*
	float a = ballShape.getPosition().y;
	float b = ballShape.getRadius() * 2;
	float c = bottomBoundary;
	*/
	bool tmp = ballShape.getPosition().y + ballShape.getRadius() * 2.f > bottomBoundary;
	if (tmp) {
		return true;
	}
	return false;
}

sf::Vector2f Ball::getCenter()
{
	return ballShape.getPosition() + sf::Vector2f(ballShape.getRadius(), ballShape.getRadius());
}

void Ball::setBoundary(float left, float right, float top, float bottom)
{
	leftBoundary = left;
	rightBoundary = right;
	topBoundary = top;
	bottomBoundary = bottom;
}
