#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

static sf::Vector2f getKeyboardInput(){
	sf::Vector2f input = { 0, 0 };

	//get y axis input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		input.y -= 100;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		input.y += 100;
	}
	else {
		input.y = 0;
	}

	//get x axis input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		input.x -= 100;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		input.x += 100;
	}
	else {
		input.x = 0;
	}

	return input;
}

static sf::Vector2f getMouseInput(sf::RenderWindow& window){
	sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
	return mouse_pos;
}