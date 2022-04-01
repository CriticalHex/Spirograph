#include<iostream>
#include<SFML\Graphics.hpp>
#include<list>

using namespace std;

class Circle
{
public:
	Circle(sf::Vector2f lastCenter, float radius, float sizeScale, float speedScale);
	~Circle();
	void rotate(sf::Vector2f lastCenter);
	sf::CircleShape circle;

private:
	float speed = .001;
	float angle = 0;
	sf::Vector2f center;
};

int main() {
	//window setup
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Spirograph");
	window.setPosition(sf::Vector2i(0, 0));
	sf::Event event;

	//variables
	int mainCircleRadius = 500;
	sf::Vector2f screenCenter(window.getSize().x / 2, window.getSize().y / 2);
	float x;
	float y;

	float scaleSize = 2;
	float scaleSpeed = .02;
	float circleCount = 20;
	
	//drawings
	sf::CircleShape mainCircle(mainCircleRadius);
	mainCircle.setOrigin(mainCircle.getRadius(), mainCircle.getRadius());
	mainCircle.setPosition(screenCenter);
	mainCircle.setPointCount(360);
	mainCircle.setFillColor(sf::Color::Transparent);
	mainCircle.setOutlineColor(sf::Color::Green);
	mainCircle.setOutlineThickness(1);

	sf::VertexArray points(sf::PrimitiveType::Points);

	list<Circle> circles;
	list<Circle>::iterator circleIT;

	for (int i = 0; i < circleCount; i++) {
		if (circles.size() == 0) {
			circles.emplace_back(screenCenter, mainCircleRadius, scaleSize, scaleSpeed + i);
		}
		else {
			auto it = circles.back();
			circles.emplace_back(it.circle.getPosition(), it.circle.getRadius(), scaleSize, scaleSpeed + i);
		}
	}

	while (window.isOpen()) {
		while (window.pollEvent(event))
		{
			// Close window:
			if (event.type == sf::Event::Closed)
				window.close();
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				window.close();
			}
		}

		auto it = circles.back();
		x = it.circle.getPosition().x;
		y = it.circle.getPosition().y;
		
		points.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::Red));


		//render----------
		window.clear();
		window.draw(mainCircle);
		for (circleIT = circles.begin(); circleIT != circles.end(); circleIT++) {
			window.draw((*circleIT).circle);
			if (circleIT != circles.begin()) {
				circleIT--;
				auto tempIT = circleIT;
				circleIT++;
				(*circleIT).rotate((*tempIT).circle.getPosition());
			}
			else {
				(*circleIT).rotate(mainCircle.getPosition());
			}
		}
		window.draw(points);
		window.display();

		//variable updates
	}

}

Circle::Circle(sf::Vector2f lastCenter, float radius, float sizeScale, float speedScale)
{	
	center = lastCenter;
	circle.setPosition(center);
	circle.setRadius(radius / sizeScale);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(sf::Color::Green);
	circle.setOutlineThickness(1);
	speed *= speedScale;
}

Circle::~Circle()
{
}

void Circle::rotate(sf::Vector2f lastCenter) {
	circle.setPosition(circle.getRadius() * cos(angle) + lastCenter.x, circle.getRadius() * sin(angle) + lastCenter.y);
	angle += speed;
}