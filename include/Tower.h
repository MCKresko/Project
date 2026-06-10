#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Bullet.h"

class Enemy;

class Tower : public GameObject {
public:
    Tower(std::string type, sf::Vector2f position);
    virtual ~Tower() = default;

    void update(float dt, std::vector<Enemy>& enemies, std::vector<Bullet>& bullets);
    void draw(sf::RenderWindow& window) override;
    bool contains(sf::Vector2f point) const;
    void upgrade();

    sf::Vector2f getPosition() const override;
    int getLevel() const;
    int getSellValue() const;
    int getUpgradeCost() const;
    std::string getType() const;

    static int costOf(std::string type);

protected:
    sf::CircleShape body;
    sf::RectangleShape barrel;
    std::string type;
    int level;
    float range;
    float damage;
    float fireRate;
    float timer;
    float barrelAngle;

    Enemy* findTarget(std::vector<Enemy>& enemies);

    virtual void setupType() = 0;
    virtual void shoot(std::vector<Bullet>& bullets, Enemy* target) = 0;
};

class GunTower : public Tower {
public:
    GunTower(sf::Vector2f position);

protected:
    void setupType() override;
    void shoot(std::vector<Bullet>& bullets, Enemy* target) override;
};

class RocketTower : public Tower {
public:
    RocketTower(sf::Vector2f position);

protected:
    void setupType() override;
    void shoot(std::vector<Bullet>& bullets, Enemy* target) override;
};

class SniperTower : public Tower {
public:
    SniperTower(sf::Vector2f position);

protected:
    void setupType() override;
    void shoot(std::vector<Bullet>& bullets, Enemy* target) override;
};
