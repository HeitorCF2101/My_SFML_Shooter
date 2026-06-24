#include <SFML/Graphics.hpp>
#include "base_obj.cpp"

class Player : public BaseObj {
    public:
        sf::CircleShape debug_sprite;
        sf::Vector2f origin = sf::Vector2f(0, 0);
        sf::Vector2f position;
		sf::Vector2f velocity;
		bool is_active = false;

		int health = 100;

        ~Player() override = default;

        void spawn(){
            is_active = true;
			debug_sprite = sf::CircleShape(10);
			debug_sprite.setFillColor(sf::Color::Green);

            position = origin;
            setOrigin(origin);

            debug_sprite.setOrigin(origin + sf::Vector2f(10, 10));
        }

		void update(float delta) override {
			position += velocity * delta;
			setPosition(position);
			debug_sprite.setPosition(position);

            if(!is_active){
                this->~Player();
            }

		}

    private:
        Player* self = this;
};