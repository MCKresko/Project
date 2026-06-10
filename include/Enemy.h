#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Enemy {
public:
    Enemy(std::string type, const std::vector<sf::Vector2f>& path);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    void takeDamage(float amount);

    bool isAlive() const;
    bool reachedBase() const;
    sf::Vector2f getPosition() const;
    int getReward() const;
    int getId() const;

private:
    sf::RectangleShape shape;
    std::vector<sf::Vector2f> path;
    int pathIndex;
    float hp;
    float maxHp;
    float speed;
    int reward;
    bool alive;
    bool atBase;
    int id;
    static int nextId;

    void setupType(std::string type);
};
