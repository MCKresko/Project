#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Map {
public:
    Map();

    void draw(sf::RenderWindow& window);
    bool canBuildAt(sf::Vector2f worldPos) const;
    sf::Vector2f snapToTileCenter(sf::Vector2f worldPos) const;
    const std::vector<sf::Vector2f>& getPath() const;

private:
    int grid[10][16];
    std::vector<sf::Vector2f> path;
};
