#include "Bullet.h"
#include "Enemy.h"
#include "Utils.h"
#include <iostream>
Bullet::Bullet(sf::Vector2f start, int targetId, float damage, float speed, bool splash, float splashRadius, float splashPercent) {
    this->targetId = targetId;
    this->damage = damage;
    this->speed = speed;
    this->splash = splash;
    this->splashRadius = splashRadius;
    this->splashPercent = splashPercent;
    alive = true;

    shape.setRadius(5);
    shape.setOrigin(sf::Vector2f(5, 5));

    if (splash) {
        
        shape.setFillColor(sf::Color(255, 120, 0)); // ракета оранжевая
    } else {
        shape.setFillColor(sf::Color::Yellow);
    }

    shape.setPosition(start);
}

void Bullet::update(float dt, std::vector<Enemy>& enemies, std::vector<Explosion>& explosions) {
    if (!alive) return;

    Enemy* target = nullptr;
    for (Enemy& enemy : enemies) {
        if (enemy.getId() == targetId && enemy.isAlive()) {
            target = &enemy;
            break;
        }
    }

    if (target == nullptr) {
        alive = false;
        return;
    }

    sf::Vector2f targetPos = target->getPosition();
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f dir = normalize(targetPos - pos);
    shape.move(dir * speed * dt);

if (distance(shape.getPosition(), targetPos) < 12) {
    sf::Vector2f explosionPos = target->getPosition();
    target->takeDamage(damage);

    if (splash) {
        explosions.push_back(Explosion(explosionPos, splashRadius));
        for (Enemy& enemy : enemies) {
            if (!enemy.isAlive()) {
                continue;
            }

            if (enemy.getId() == targetId) {
                continue;
            }

            float dist = distance(explosionPos, enemy.getPosition());

            if (dist <= splashRadius) {
                enemy.takeDamage(damage * splashPercent);
            }
        }
    }

    alive = false;
}
}

void Bullet::draw(sf::RenderWindow& window) {
    if (alive) window.draw(shape);
}

bool Bullet::isAlive() const {
    return alive;
}
