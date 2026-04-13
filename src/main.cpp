#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

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
	PLAYER_WALK_SPEED = 2,
	PLAYER_RUN_SPEED = 4,
	BULLET_SPEED = 200,

};

static float get_delta_time() {
	static auto last_time = std::chrono::high_resolution_clock::now();
	auto current_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> delta = current_time - last_time;
	last_time = current_time;
	return delta.count();
}

class Base_Object {
	public:
		sf::Vector2f origin;
		sf::Vector2f velocity;
		sf::Vector2f position;
		sf::Vector2f rotation;
		//sf::Sprite sprite;
		sf::CircleShape db_sprite;


		void update() {
			position += velocity;
			db_sprite.setPosition(position);
		}

};

class Character : public Base_Object {
	public:
		//float health;
		//float stamina;

		void init() {
			origin = { 0, 0 };
			velocity = { 0, 0 };
			position = { 0, 0 };
			rotation = { 0, 0 };

			db_sprite.setRadius(16);
			db_sprite.setFillColor(sf::Color::Green);
			db_sprite.setOrigin({ 16, 16 });
			db_sprite.setPosition(origin);
			db_sprite.setScale({ 1, 1 });
			//db_sprite.setRotation(0);

			//sprite.setPosition(origin);
			//sprite.setScale({ 1, 1 });
			//sprite.setRotation(0);
			//sprite.setTextureRect({ 0, 0 }, { 32, 32 });
			//sprite.setTexture();

			//health = 100;
			//stamina = 100;
		}
};

class Player : public Character {
public:

	sf::CircleShape crosshair;
	bool is_aimming = false;

	void spawn() {
		init();
		origin = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		db_sprite.setPosition(origin);
		Player* self_ptr = this;
	}

	void aim() {
		//calculate the angle between the player and the mouse cursor
		//rotate the player sprite to face the mouse cursor
		is_aimming = true;

		crosshair.setRadius(4);
		crosshair.setFillColor(sf::Color::Blue);

		//getting relative mouse position to the center of the screen to smoothly rotate the player sprite towards the mouse
		sf::Vector2f mouse_relative = sf::Vector2f(sf::Mouse::getPosition());
		mouse_relative.x -= SCREEN_WIDTH / 4;
		mouse_relative.y -= SCREEN_HEIGHT / 4;

		sf::Vector2f direction = { mouse_relative.x - position.x, mouse_relative.y - position.y };

		float angle = std::atan2(direction.y, direction.x);
		sf::Angle a = sf::degrees(angle * 180 / 3.14159265);

		direction = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
		direction = direction * 128.0f;

		crosshair.setPosition(direction + position);
	}

};

class Enemy : public Character {
	public:

		bool is_alive;
		bool taget_spotted;
		bool target_in_range;
		//bool target_in_sight;
		//bool can_call_alarm;
		//bool is_attacking;

		//void attack(){}
		//void die(){}
		//void call_alarm(){}

};

class Bullet : public Base_Object {
	public:
		sf::RectangleShape db_sprite;
		bool is_active = false;

		void init() {
			is_active = false;
			db_sprite.setSize({ 8, 8 });
			db_sprite.setFillColor(sf::Color::Red);
			db_sprite.setOrigin({ 4, 4 });
			db_sprite.setPosition(origin);
			db_sprite.setScale({ 1, 1 });
			//db_sprite.setRotation(0);
		}


		void fire(Player& player) {
			is_active = true;
			position = player.position;
			velocity = { std::cos(player.rotation.x)* BULLET_SPEED, std::sin(player.rotation.y)* BULLET_SPEED };
			db_sprite.setPosition(position);
		}

};

static void handle_input(Player& player, sf::RenderWindow& window) {
	//movement input
	//y axis
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		player.velocity.y = -PLAYER_WALK_SPEED;
		player.update();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		player.velocity.y = PLAYER_WALK_SPEED;
		player.update();
	}
	else {
		player.velocity.y = 0;
		player.update();
	}

	//x axis
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		player.velocity.x = -PLAYER_WALK_SPEED;
		player.update();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		player.velocity.x = PLAYER_WALK_SPEED;
		player.update();
	}
	else {
		player.velocity.x = 0;
		player.update();
	}

	//aiming input
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		player.aim();
	}
	else {
		player.is_aimming = false;
	}

}

static void render(Player& player) {

	//window creation and configuration
	sf::RenderWindow window(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "My Shooter 2D");
	window.setVerticalSyncEnabled(VSYNC_ENABLED);
	window.setFramerateLimit(MAX_FPS);
	window.setMouseCursorGrabbed(true);
	//window.setMouseCursorVisible(false);

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
				window.close();
		}

		//input handling function
		handle_input(player, window);

		window.clear();

		if (player.is_aimming) {
			window.draw(player.crosshair);
		}

		window.draw(player.db_sprite);
		window.display();
	}

}

int main(){

	//game initialization
	Player player;
	player.spawn();

	//render loop
	std::thread render_thread(render, player);

	render_thread.join();

}
