#pragma once
#include <SFML/Graphics.hpp>

// Base class for all visible game objects.
// It gives a common interface for drawing objects
// and getting their position.
class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::Vector2f getPosition() const = 0;
};