#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Enemy;

class AssetSystem {
public:
    AssetSystem();

    void update(float dt);
    void rocketStrike(sf::Vector2f pos, std::vector<Enemy>& enemies);
    void drawEffects(sf::RenderWindow& window);

    float getRocketCooldown() const;

private:
    float rocketCooldown;
    sf::CircleShape lastExplosion;
    float effectTimer;
};
