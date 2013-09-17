#ifndef ENTITIES_H_
#define ENTITIES_H_

#include "global.h"
#include <vector>

bool intersects (const sf::RectangleShape& rect1, const sf::RectangleShape& rect2);
float clamp (const float x, const float a, const float b);

class Brick : public sf::RectangleShape {
public:
	unsigned int armor;
	Brick();
	Brick(const sf::Vector2f & size);
	bool collide(sf::RectangleShape& ball, sf::Vector2f & ballSpeed);
};

class Grid: public sf::FloatRect {
private:
	unsigned int rows;
	unsigned int columns;

	std::vector<Brick> * bricks;
public:
	Grid();
	~Grid();

	void setDimensions(unsigned int col, unsigned int row);
	sf::Vector2f cellSize();
	void init();
	void display(sf::RenderWindow& window);
	bool collide(sf::RectangleShape& ball, sf::Vector2f& ballSpeed, unsigned int& score);
	bool isGameWon();
};

#endif