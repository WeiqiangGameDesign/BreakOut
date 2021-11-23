#include "Brick.h"
#include <SFML/Graphics.hpp>



Brick::Brick(float width, float height, sf::Vector2f pos)
{
	sf::RectangleShape tmp(sf::Vector2f(width, height));
	brickShape = tmp;
	brickShape.setPosition(pos);
}

sf::Vector2f Brick::getCenter()
{
	return brickShape.getPosition() + brickShape.getSize() / 2.f;
}
