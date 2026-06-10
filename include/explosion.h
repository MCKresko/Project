#pragma once
#include <SFML/Graphics.hpp>

class Explosion {
public:
    Explosion(sf::Vector2f position, float radius);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isAlive() const;

private:
    sf::CircleShape shape;
    float lifetime;
    float maxLifetime;
    float maxRadius;
};
