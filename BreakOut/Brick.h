#pragma once
#include <SFML/Graphics.hpp>

class Brick {

public:
	Brick(float width, float height, sf::Vector2f pos);
	sf::RectangleShape brickShape;

	sf::Vector2f getCenter();
	
	bool isDestoryed;

};
