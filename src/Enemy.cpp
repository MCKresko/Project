#include "Enemy.h"
#include "Utils.h"

int Enemy::nextId = 1;

Enemy::Enemy(std::string type, const std::vector<sf::Vector2f>& path) {
    this->path = path;
    pathIndex = 1;
    alive = true;
    atBase = false;
    id = nextId++;

    shape.setSize(sf::Vector2f(30, 30));
    shape.setOrigin(sf::Vector2f(15, 15));
    if (!path.empty()) shape.setPosition(path[0]);

    setupType(type);
}

void Enemy::setupType(std::string type) {
    if (type == "golem") {
        maxHp = 280; speed = 45; reward = 35; shape.setFillColor(sf::Color(120, 120, 120));
    } else if (type == "spider") {
        maxHp = 145; speed = 115; reward = 18; shape.setFillColor(sf::Color(170, 40, 200));
    } else if (type == "drone") {
        maxHp = 170; speed = 95; reward = 25; shape.setFillColor(sf::Color(60, 180, 220));
    } else {
        maxHp = 180; speed = 70; reward = 20; shape.setFillColor(sf::Color(180, 60, 60));
    }
    hp = maxHp;
}

void Enemy::update(float dt) {
    if (!alive || atBase || pathIndex >= (int)path.size()) return;

    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f target = path[pathIndex];
    sf::Vector2f dir = normalize(target - pos);
    shape.move(dir * speed * dt);

    if (distance(shape.getPosition(), target) < 6) {
        shape.setPosition(target);
        pathIndex++;
        if (pathIndex >= (int)path.size()) {
            atBase = true;
            alive = false;
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    if (!alive) return;

    window.draw(shape);

    sf::RectangleShape back(sf::Vector2f(34, 5));
    back.setFillColor(sf::Color(70, 70, 70));
    back.setPosition(sf::Vector2f(shape.getPosition().x - 17, shape.getPosition().y - 25));
    window.draw(back);

    float percent = hp / maxHp;
    sf::RectangleShape bar(sf::Vector2f(34 * percent, 5));
    bar.setFillColor(sf::Color::Green);
    bar.setPosition(back.getPosition());
    window.draw(bar);
}

void Enemy::takeDamage(float amount) {
    hp -= amount;
    if (hp <= 0) alive = false;
}

bool Enemy::isAlive() const { return alive; }
bool Enemy::reachedBase() const { return atBase; }
sf::Vector2f Enemy::getPosition() const { return shape.getPosition(); }
int Enemy::getReward() const { return reward; }
int Enemy::getId() const { return id; }
