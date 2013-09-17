#include "entities.h"

using namespace sf;

bool intersects (const RectangleShape & rect1,const RectangleShape & rect2)
{
    FloatRect r1=rect1.getGlobalBounds();
    FloatRect r2=rect2.getGlobalBounds();
    return r1.intersects (r2);
}
 
float clamp (const float x, const float a, const float b)
{
    return std::min(std::max(a,x),b);
}

//Brick class -------------------------------------------------
Brick::Brick() : RectangleShape() {
	armor = 2;
}

Brick::Brick (const Vector2f& size) : RectangleShape(size) {
	armor = 2;
}

bool Brick::collide(RectangleShape& ball, Vector2f & ballSpeed) {
	if (intersects(ball, *this)) {
		FloatRect r = getGlobalBounds();
		FloatRect b = ball.getGlobalBounds();

		Vector2f o = Vector2f(r.left + r.width/2, r.top + r.height/2);
		Vector2f om = Vector2f(b.left + b.width/2 - o.x, b.top + b.height/2 - o.y);
		om.x /= r.width;
		om.y /= r.height;

		float angle = atan2(om.y, om.x);
		if ( angle >= PI/4 && angle <= 3*PI/4) { //bottom
			ballSpeed.y = abs(ballSpeed.y);
			float u = r.top + r.height - b.top;
			ball.move(0, 2*u);
			return true;
		}
		if ( angle <= -PI/4 && angle >= -3*PI/4) { //top
			ballSpeed.y = -abs(ballSpeed.y);
			float u = b.top + b.height - r.top;
			ball.move(0, -2*u);
			return true;
		}
		if ( abs(angle) >= 3*PI/4) { //left
			ballSpeed.x = -abs(ballSpeed.x);
			float u = b.left + b.width - r.left;
			ball.move(-2*u, 0);
			return true;
		}
		if ( abs(angle) <= PI/4) { //right
			ballSpeed.x = abs(ballSpeed.x);
			float u = r.left + r.width - b.left;
			ball.move(2*u, 0);
			return true;
		}
	}
	return false;
}

//Grid Class ------------------------------------------------
Grid::Grid() {
	rows = 0;
	columns = 0;
	bricks = NULL;
}

Grid::~Grid() {
	if (bricks != NULL)
		delete bricks;
}

void Grid::setDimensions(unsigned int col, unsigned int row) {
	rows = row;
	columns = col;
	if (bricks != NULL)
		delete bricks;

	bricks = new std::vector<Brick> (rows*columns);
	init();
}

sf::Vector2f Grid::cellSize() {
	return Vector2f(width/columns - 4, height/rows - 4);
}
void Grid::init() {
	Vector2f cellS = cellSize();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			Brick& current = bricks->at(i+j*rows);
			current.setSize(cellS);
			current.setFillColor(Color::Blue);
			current.setOutlineColor(Color::Black);
			current.setOutlineThickness(2);
			current.setPosition(left + j*cellS.x + 2*j, top + i*cellS.y + 2*i);
		}
	}
}

void Grid::display(sf::RenderWindow& window) {
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++) {
			if (bricks->at(i+j*rows).armor <= 0)
				continue;
			window.draw(bricks->at(i+j*rows));
		}
}

bool Grid::collide(sf::RectangleShape& ball, sf::Vector2f& ballSpeed, unsigned int& score) {
	for (int i = 0; i<rows; i++) {
		for (int j = 0; j < columns; j++) {
			Brick& current = bricks->at(i + j*rows);
			if (current.armor <= 0)
				continue;
			if ( current.collide(ball, ballSpeed) ) {
				current.armor--;
				current.setFillColor(Color::Green);
				if (current.armor == 0)
					score += 700;

				return true;
			}
		}
	}
	return false;
}

bool Grid::isGameWon() {
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < columns; j++)
			if (bricks->at(i + j*rows).armor > 0)
				return false;
	return true;
}