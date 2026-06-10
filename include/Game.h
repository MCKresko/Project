#pragma once
#include "GameObject.h"
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "explosion.h"
#include "Map.h"
#include "Tower.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Asset.h"
#include "WaveManager.h"

class Game {
public:
    Game();
    void run();

private:
    enum class ScreenState {
        MainMenu,
        LevelSelect,
        Settings,
        Playing
    };

    enum class MenuState {
        None,
        Build,
        Tower
    };

    sf::RenderWindow window;
    sf::Font font;

    Map map;
    AssetSystem assets;
    WaveManager waves;

    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<Explosion> explosions;
    std::vector<GameObject*> gameObjects;

    ScreenState screenState;
    MenuState menuState;

    sf::RectangleShape upgradeButton;
    sf::RectangleShape sellButton;
    sf::Text upgradeText;
    sf::Text sellText;

    sf::RectangleShape gunBuildButton;
    sf::RectangleShape rocketBuildButton;
    sf::RectangleShape sniperBuildButton;
    sf::Text gunBuildText;
    sf::Text rocketBuildText;
    sf::Text sniperBuildText;
    sf::Vector2f buildMenuPos;

    sf::RectangleShape playButton;
    sf::RectangleShape settingsButton;
    sf::RectangleShape exitButton;
    sf::RectangleShape level1Button;
    sf::RectangleShape level2Button;
    sf::RectangleShape level3Button;
    sf::RectangleShape levelBackButton;
    sf::RectangleShape fpsButton;
    sf::RectangleShape frameLimitButton;
    sf::RectangleShape settingsBackButton;

    int baseHp;
    int money;
    bool paused;
    bool gameOver;
    bool victory;
    std::string selectedTowerType;
    int selectedTowerIndex;
    int currentLevel;

    bool showFps;
    bool frameLimitEnabled;
    float fpsTimer;
    int fpsCounter;
    int currentFps;

    void processEvents();
    void update(float dt);
    void updateFps(float dt);
    void render();

    void handleMouseClick(sf::Vector2f mousePos);
    void handleHotkey(sf::Keyboard::Key key);
    void handleMainMenuClick(sf::Vector2f mousePos);
    void handleLevelSelectClick(sf::Vector2f mousePos);
    void handleSettingsClick(sf::Vector2f mousePos);
    void rebuildGameObjectList();

    void buildTower(sf::Vector2f pos, std::string type);
    bool tileHasTower(sf::Vector2f tileCenter) const;
    int findTowerAtTile(sf::Vector2f pos);
    void cleanObjects();
    void resetGameplay();
    void startLevel(int level);

    void setupButtons();
    void setupButton(sf::RectangleShape& button, sf::Vector2f position, sf::Vector2f size, sf::Color color);
    void drawButton(const sf::RectangleShape& button, const std::string& text, int textSize = 22);
    void drawUI();
    void drawMainMenu();
    void drawLevelSelect();
    void drawSettings();
    void drawText(const std::string& text, float x, float y, int size = 18);
};
