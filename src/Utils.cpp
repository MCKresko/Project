#include "Utils.h"

float length(sf::Vector2f v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalize(sf::Vector2f v) {
    float len = length(v);
    if (len == 0) return sf::Vector2f(0, 0);
    return sf::Vector2f(v.x / len, v.y / len);
}

float distance(sf::Vector2f a, sf::Vector2f b) {
    return length(a - b);
}
