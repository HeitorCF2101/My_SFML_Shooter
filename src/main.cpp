#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "player.cpp"

enum Config{
	//window and video configuration
	SCREEN_WIDTH = 1280,
	SCREEN_HEIGHT = 720,
	VSYNC_ENABLED = false, //60.0 Hz is the Refresh Rate of the monitor, so 60 FPS is the optimal framerate for VSync.
	MAX_FPS = 60,
	IS_GAME_RUNNING = true,

	//performance configuration
	MAX_BULLETS = 100,
	MAX_ENEMIES = 50,

	//game logic configuration
	PLAYER_WALK_SPEED = 200,
	PLAYER_RUN_SPEED = 400,
	BULLET_SPEED = 10000,

};

class Bullet : public BaseObj {
public:
	sf::CircleShape debug_sprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	bool is_active = false;
	int damage = 10;
	~Bullet() override = default;
	void spawn(sf::Vector2f spawn_position, sf::RenderWindow& window) {
		is_active = true;
		debug_sprite = sf::CircleShape(5);
		debug_sprite.setFillColor(sf::Color::Red);
		position = spawn_position;

		setPosition(position);
		debug_sprite.setPosition(position);

		sf::Vector2f dir = sf::Vector2f(float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y));
		dir.x -= position.x;
		dir.y -= position.y;

		dir = dir.normalized();

		velocity = sf::Vector2f(dir.x * BULLET_SPEED, dir.y * BULLET_SPEED);

	}
	void update(float delta) override {
		position += velocity * delta;
		setPosition(position);
		debug_sprite.setPosition(position);
		if (!is_active) {
			this->~Bullet();
		}
	}
private:
	Bullet* self = this;
};

static float get_delta_time() {
	static auto last_time = std::chrono::high_resolution_clock::now();
	auto current_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> delta = current_time - last_time;
	last_time = current_time;
	return delta.count();
}

static void handle_input(Player& player, sf::RenderWindow& window) {
	//movement input
	//y axis
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		player.velocity.y = -PLAYER_WALK_SPEED;
		player.update(get_delta_time());
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		player.velocity.y = PLAYER_WALK_SPEED;
		player.update(get_delta_time());
	}
	else {
		player.velocity.y = 0;
		player.update(get_delta_time());
	}

	//x axis
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		player.velocity.x = -PLAYER_WALK_SPEED;
		player.update(get_delta_time());
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		player.velocity.x = PLAYER_WALK_SPEED;
		player.update(get_delta_time());
	}
	else {
		player.velocity.x = 0;
		player.update(get_delta_time());
	}

}

static void render(Player& player) {

	//window creation and configuration
	sf::RenderWindow window(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "My Shooter 2D");
	window.setVerticalSyncEnabled(VSYNC_ENABLED);
	window.setFramerateLimit(MAX_FPS);
	window.setMouseCursorGrabbed(true);
	//window.setMouseCursorVisible(false);

	player.spawn();

	Bullet bullet;

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
				window.close();
		}

		//input handling function
		handle_input(player, window);

		window.clear();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			bullet.spawn(player.getPosition(), window);
		}

		if(bullet.is_active){
			window.draw(bullet.debug_sprite);
			bullet.update(get_delta_time());
		}
		window.draw(player.debug_sprite);
		window.display();
	}

}

int main(){

	//game initialization
	Player player;

	//render loop
	std::thread render_thread(render, player);

	render_thread.join();

}