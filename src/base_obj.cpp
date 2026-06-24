#include <SFML/Graphics.hpp>

class BaseObj : public sf::Transformable {
	public:
		~BaseObj() = default;
		virtual void update(float delta) = 0;
		bool is_active = false;
};