#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Explosion.h"
class Enemy;

class Bullet {
public:
    Bullet(sf::Vector2f start, int targetId, float damage, float speed,
           bool splash = false, float splashRadius = 0.f, float splashPercent = 0.f);

void update(float dt, std::vector<Enemy>& enemies, std::vector<Explosion>& explosions);
    void draw(sf::RenderWindow& window);
    bool isAlive() const;

private:
    sf::CircleShape shape;

    int targetId;
    float damage;
    float speed;
    bool alive;

    bool splash;
    float splashRadius;
    float splashPercent;
};