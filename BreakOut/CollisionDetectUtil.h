#pragma once


#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <math.h>


static class CollisionDetectUtil {

private:
	static sf::Vector2f vfabs(sf::Vector2f v) {
		sf::Vector2f res;
		res.x = fabs(v.x);
		res.y = fabs(v.y);
		return res;
	}

	static sf::Vector2f vmax(sf::Vector2f v) {
		sf::Vector2f res;
		res.x = v.x > 0 ? v.x : 0;
		res.y = v.y > 0 ? v.y : 0;
		return res;
	}

	static bool vShorter(sf::Vector2f u, float r) {
		float ul = u.x * u.x + u.y * u.y;
		return ul <= r * r;
	}

	static bool isBoxCirlcleIntersected(sf::Vector2f rc, sf::Vector2f hl, sf::Vector2f cc, float r) {
		sf::Vector2f v = vfabs((cc - rc));
		sf::Vector2f u = vmax((v - hl));
		return vShorter(u, r);
	}

public:

	/*
		1. rc = center of rectangle
		2. hl = half diagnoal length of rectangle
		3. cc = center of circle
		4. r = radius of circle
	*/
	static bool isBoxCirlcleIntersected(sf::RectangleShape rect, sf::CircleShape circle) {
		sf::Vector2f toCenter = rect.getSize();
		toCenter.x /= 2;
		toCenter.y /= 2;
		sf::Vector2f rc = rect.getPosition() + toCenter;
		sf::Vector2f hl = toCenter;
		sf::Vector2f cc = circle.getPosition() + sf::Vector2f(circle.getRadius(), circle.getRadius());
		float r = circle.getRadius();

		return isBoxCirlcleIntersected(rc, hl, cc, r);
	}

	static sf::Vector2f recoilVel(sf::Vector2f rc, sf::Vector2f cc) {
		sf::Vector2f pos = cc - rc;
		return pos;
	}

};
