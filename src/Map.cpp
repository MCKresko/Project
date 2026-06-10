#include "Map.h"
#include "Config.h"

Map::Map() {
    int data[10][16] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1},
        {0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1},
        {0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1},
        {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) grid[y][x] = data[y][x];
    }

    path.push_back(sf::Vector2f(0 * TILE_SIZE + 25, 1 * TILE_SIZE + 25));
    path.push_back(sf::Vector2f(3 * TILE_SIZE + 25, 1 * TILE_SIZE + 25));
    path.push_back(sf::Vector2f(3 * TILE_SIZE + 25, 4 * TILE_SIZE + 25));
    path.push_back(sf::Vector2f(8 * TILE_SIZE + 25, 4 * TILE_SIZE + 25));
    path.push_back(sf::Vector2f(8 * TILE_SIZE + 25, 2 * TILE_SIZE + 25));
    path.push_back(sf::Vector2f(15 * TILE_SIZE + 25, 2 * TILE_SIZE + 25));
    path.push_back(sf::Vector2f(15 * TILE_SIZE + 25, 5 * TILE_SIZE + 25));
    path.push_back(sf::Vector2f(10 * TILE_SIZE + 25, 5 * TILE_SIZE + 25));
    path.push_back(sf::Vector2f(10 * TILE_SIZE + 25, 8 * TILE_SIZE + 25));
    path.push_back(sf::Vector2f(15 * TILE_SIZE + 25, 8 * TILE_SIZE + 25));
}

void Map::draw(sf::RenderWindow& window) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE - 1, TILE_SIZE - 1));
            tile.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));

            if (grid[y][x] == 1) tile.setFillColor(sf::Color(150, 150, 150));
            else tile.setFillColor(sf::Color(45, 120, 55));

            window.draw(tile);
        }
    }

    sf::RectangleShape base(sf::Vector2f(48, 48));
    base.setFillColor(sf::Color(220, 40, 40));
    base.setPosition(sf::Vector2f(15 * TILE_SIZE, 8 * TILE_SIZE));
    window.draw(base);
}

bool Map::canBuildAt(sf::Vector2f worldPos) const {
    int x = (int)worldPos.x / TILE_SIZE;
    int y = (int)worldPos.y / TILE_SIZE;

    if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT) return false;
    return grid[y][x] == 0;
}

sf::Vector2f Map::snapToTileCenter(sf::Vector2f worldPos) const {
    int x = (int)worldPos.x / TILE_SIZE;
    int y = (int)worldPos.y / TILE_SIZE;
    return sf::Vector2f(x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2);
}

const std::vector<sf::Vector2f>& Map::getPath() const { return path; }
