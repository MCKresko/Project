#include "Game.h"
#include "Config.h"
#include "Utils.h"
#include <algorithm>
#include <optional>
#include <memory>

Game::Game()
    : window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Robot Tower Defense"),
      upgradeText(font, "Upgrade", 16),
      sellText(font, "Sell", 16),
      gunBuildText(font, "Gun $60", 16),
      rocketBuildText(font, "Rocket $90", 16),
      sniperBuildText(font, "Sniper $110", 16),
      screenState(ScreenState::MainMenu),
      menuState(MenuState::None)
{
    frameLimitEnabled = true;
    window.setFramerateLimit(60);

    if (!font.openFromFile("assets/fonts/Roboto-Regular.ttf")) {
        window.close();
    }

    baseHp = 100;
    money = 220;
    paused = false;
    gameOver = false;
    victory = false;
    selectedTowerType = "gun";
    selectedTowerIndex = -1;
    currentLevel = 1;

    showFps = false;
    fpsTimer = 0.f;
    fpsCounter = 0;
    currentFps = 0;

    setupButtons();
}

void Game::setupButtons() {
    setupButton(gunBuildButton, {0.f, 0.f}, {120.f, 32.f}, sf::Color(70, 70, 150));
    setupButton(rocketBuildButton, {0.f, 0.f}, {120.f, 32.f}, sf::Color(150, 90, 40));
    setupButton(sniperBuildButton, {0.f, 0.f}, {120.f, 32.f}, sf::Color(60, 130, 60));

    gunBuildText.setFillColor(sf::Color::White);
    rocketBuildText.setFillColor(sf::Color::White);
    sniperBuildText.setFillColor(sf::Color::White);

    setupButton(upgradeButton, {0.f, 0.f}, {120.f, 32.f}, sf::Color(60, 120, 60));
    setupButton(sellButton, {0.f, 0.f}, {120.f, 32.f}, sf::Color(150, 60, 60));

    upgradeText.setFillColor(sf::Color::White);
    sellText.setFillColor(sf::Color::White);

    float x = WINDOW_WIDTH / 2.f - 120.f;
    float y = 210.f;

    setupButton(playButton, {x, y}, {240.f, 48.f}, sf::Color(55, 130, 70));
    setupButton(settingsButton, {x, y + 65.f}, {240.f, 48.f}, sf::Color(145, 105, 45));
    setupButton(exitButton, {x, y + 130.f}, {240.f, 48.f}, sf::Color(160, 65, 65));

    setupButton(level1Button, {x, y}, {240.f, 48.f}, sf::Color(70, 80, 160));
    setupButton(level2Button, {x, y + 65.f}, {240.f, 48.f}, sf::Color(70, 80, 160));
    setupButton(level3Button, {x, y + 130.f}, {240.f, 48.f}, sf::Color(70, 80, 160));
    setupButton(levelBackButton, {x, y + 215.f}, {240.f, 48.f}, sf::Color(145, 65, 65));

    setupButton(fpsButton, {x, y + 20.f}, {240.f, 48.f}, sf::Color(70, 80, 160));
    setupButton(frameLimitButton, {x, y + 85.f}, {240.f, 48.f}, sf::Color(70, 80, 160));
    setupButton(settingsBackButton, {x, y + 180.f}, {240.f, 48.f}, sf::Color(145, 65, 65));
}

void Game::setupButton(sf::RectangleShape& button, sf::Vector2f position, sf::Vector2f size, sf::Color color) {
    button.setSize(size);
    button.setPosition(position);
    button.setFillColor(color);
    button.setOutlineThickness(2.f);
    button.setOutlineColor(sf::Color::Black);
}

void Game::resetGameplay() {
    map = Map();
    assets = AssetSystem();
    waves = WaveManager();

    towers.clear();
    enemies.clear();
    bullets.clear();
    explosions.clear();

    baseHp = 100;
    money = 220;
    paused = false;
    gameOver = false;
    victory = false;
    selectedTowerType = "gun";
    selectedTowerIndex = -1;
    menuState = MenuState::None;
}

void Game::startLevel(int level) {
    resetGameplay();

    currentLevel = level;
    screenState = ScreenState::Playing;

    money = 220;
    baseHp = 100;
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        updateFps(dt);
        processEvents();

        if (screenState == ScreenState::Playing && !paused && !gameOver && !victory) {
            update(dt);
        }

        render();
    }
}

void Game::updateFps(float dt) {
    fpsTimer += dt;
    fpsCounter++;

    if (fpsTimer >= 1.f) {
        currentFps = fpsCounter;
        fpsCounter = 0;
        fpsTimer = 0.f;
    }
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButton->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                handleMouseClick(mousePos);
            }
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            handleHotkey(keyPressed->code);
        }
    }
}

void Game::handleHotkey(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Key::Escape) {
        if (screenState == ScreenState::LevelSelect || screenState == ScreenState::Settings) {
            screenState = ScreenState::MainMenu;
            return;
        }

        if (screenState == ScreenState::Playing) {
            screenState = ScreenState::MainMenu;
            menuState = MenuState::None;
            selectedTowerIndex = -1;
            return;
        }
    }

    if (screenState != ScreenState::Playing) {
        return;
    }

    if (key == sf::Keyboard::Key::Space && !gameOver && !victory) {
        waves.startNextWave();
    }

    if (key == sf::Keyboard::Key::P && !gameOver && !victory) {
        paused = !paused;
    }

    if (key == sf::Keyboard::Key::Delete && selectedTowerIndex >= 0) {
        money += towers[selectedTowerIndex]->getSellValue();
        towers.erase(towers.begin() + selectedTowerIndex);
        selectedTowerIndex = -1;
        menuState = MenuState::None;
    }

    if (key == sf::Keyboard::Key::R && (gameOver || victory)) {
        startLevel(currentLevel);
    }
}

void Game::handleMouseClick(sf::Vector2f mousePos) {
    if (screenState == ScreenState::MainMenu) {
        handleMainMenuClick(mousePos);
        return;
    }

    if (screenState == ScreenState::LevelSelect) {
        handleLevelSelectClick(mousePos);
        return;
    }

    if (screenState == ScreenState::Settings) {
        handleSettingsClick(mousePos);
        return;
    }

    if (gameOver || victory) {
        return;
    }

    if (menuState == MenuState::Tower && selectedTowerIndex >= 0) {
        if (upgradeButton.getGlobalBounds().contains(mousePos)) {
            int cost = towers[selectedTowerIndex]->getUpgradeCost();

            if (cost > 0 && money >= cost) {
                money -= cost;
                towers[selectedTowerIndex]->upgrade();
            }

            return;
        }

        if (sellButton.getGlobalBounds().contains(mousePos)) {
            money += towers[selectedTowerIndex]->getSellValue();
            towers.erase(towers.begin() + selectedTowerIndex);
            selectedTowerIndex = -1;
            menuState = MenuState::None;
            return;
        }
    }

    if (menuState == MenuState::Build) {
        if (gunBuildButton.getGlobalBounds().contains(mousePos)) {
            buildTower(buildMenuPos, "gun");
            menuState = MenuState::None;
            return;
        }

        if (rocketBuildButton.getGlobalBounds().contains(mousePos)) {
            buildTower(buildMenuPos, "rocket");
            menuState = MenuState::None;
            return;
        }

        if (sniperBuildButton.getGlobalBounds().contains(mousePos)) {
            buildTower(buildMenuPos, "sniper");
            menuState = MenuState::None;
            return;
        }
    }

    if (mousePos.x < WINDOW_WIDTH &&
        mousePos.y < WINDOW_HEIGHT &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
        assets.rocketStrike(mousePos, enemies);
        menuState = MenuState::None;
        return;
    }

    if (mousePos.x >= 0 &&
        mousePos.x < MAP_WIDTH * TILE_SIZE &&
        mousePos.y >= 0 &&
        mousePos.y < MAP_HEIGHT * TILE_SIZE) {

        sf::Vector2f center = map.snapToTileCenter(mousePos);
        selectedTowerIndex = findTowerAtTile(mousePos);

        if (selectedTowerIndex != -1) {
            sf::Vector2f towerPos = towers[selectedTowerIndex]->getPosition();

            upgradeButton.setPosition({towerPos.x + 28.f, towerPos.y - 38.f});
            sellButton.setPosition({towerPos.x + 28.f, towerPos.y + 2.f});

            upgradeText.setPosition({towerPos.x + 36.f, towerPos.y - 32.f});
            sellText.setPosition({towerPos.x + 36.f, towerPos.y + 8.f});

            menuState = MenuState::Tower;
            return;
        }

        if (!map.canBuildAt(mousePos) || tileHasTower(center)) {
            menuState = MenuState::None;
            return;
        }

        buildMenuPos = mousePos;

        float menuX = center.x + 25.f;
        float menuY = center.y - 50.f;

        gunBuildButton.setPosition({menuX, menuY});
        rocketBuildButton.setPosition({menuX, menuY + 38.f});
        sniperBuildButton.setPosition({menuX, menuY + 76.f});

        gunBuildText.setPosition({menuX + 8.f, menuY + 6.f});
        rocketBuildText.setPosition({menuX + 8.f, menuY + 44.f});
        sniperBuildText.setPosition({menuX + 8.f, menuY + 82.f});

        menuState = MenuState::Build;
        return;
    }

    menuState = MenuState::None;
    selectedTowerIndex = -1;
}

void Game::handleMainMenuClick(sf::Vector2f mousePos) {
    if (playButton.getGlobalBounds().contains(mousePos)) {
        screenState = ScreenState::LevelSelect;
        return;
    }

    if (settingsButton.getGlobalBounds().contains(mousePos)) {
        screenState = ScreenState::Settings;
        return;
    }

    if (exitButton.getGlobalBounds().contains(mousePos)) {
        window.close();
        return;
    }
}

void Game::handleLevelSelectClick(sf::Vector2f mousePos) {
    if (level1Button.getGlobalBounds().contains(mousePos)) {
        startLevel(1);
        return;
    }

    if (level2Button.getGlobalBounds().contains(mousePos)) {
        startLevel(2);
        return;
    }

    if (level3Button.getGlobalBounds().contains(mousePos)) {
        startLevel(3);
        return;
    }

    if (levelBackButton.getGlobalBounds().contains(mousePos)) {
        screenState = ScreenState::MainMenu;
        return;
    }
}

void Game::handleSettingsClick(sf::Vector2f mousePos) {
    if (fpsButton.getGlobalBounds().contains(mousePos)) {
        showFps = !showFps;
        return;
    }

    if (frameLimitButton.getGlobalBounds().contains(mousePos)) {
        frameLimitEnabled = !frameLimitEnabled;
        window.setFramerateLimit(frameLimitEnabled ? 60 : 0);
        return;
    }

    if (settingsBackButton.getGlobalBounds().contains(mousePos)) {
        screenState = ScreenState::MainMenu;
        return;
    }
}

void Game::buildTower(sf::Vector2f pos, std::string type) {
    if (!map.canBuildAt(pos)) {
        return;
    }

    sf::Vector2f center = map.snapToTileCenter(pos);

    if (tileHasTower(center)) {
        return;
    }

    int cost = Tower::costOf(type);

    if (money < cost) {
        return;
    }

    money -= cost;

    if (type == "rocket") {
        towers.push_back(std::make_unique<RocketTower>(center));
    } else if (type == "sniper") {
        towers.push_back(std::make_unique<SniperTower>(center));
    } else {
        towers.push_back(std::make_unique<GunTower>(center));
    }
}

bool Game::tileHasTower(sf::Vector2f tileCenter) const {
    for (const auto& tower : towers) {
        if (tower->getPosition() == tileCenter) {
            return true;
        }
    }

    return false;
}

int Game::findTowerAtTile(sf::Vector2f pos) {
    rebuildGameObjectList();

    sf::Vector2f center = map.snapToTileCenter(pos);

    for (int i = 0; i < static_cast<int>(gameObjects.size()); i++) {
        Tower* tower = dynamic_cast<Tower*>(gameObjects[i]);

        if (tower != nullptr && distance(tower->getPosition(), center) < 5.f) {
            return i;
        }
    }

    return -1;
}

void Game::update(float dt) {
    assets.update(dt);
    waves.update(dt, enemies, map.getPath());

    for (auto& tower : towers) {
        tower->update(dt, enemies, bullets);
    }

    for (Enemy& enemy : enemies) {
        enemy.update(dt);
    }

    for (Bullet& bullet : bullets) {
        bullet.update(dt, enemies, explosions);
    }

    for (Enemy& enemy : enemies) {
        if (enemy.reachedBase()) {
            baseHp -= 10;
        }
    }

    cleanObjects();

    if (baseHp <= 0) {
        gameOver = true;
    }

    if (waves.allWavesFinished() && enemies.empty()) {
        victory = true;
    }

    for (Explosion& explosion : explosions) {
        explosion.update(dt);
    }

    explosions.erase(
        std::remove_if(
            explosions.begin(),
            explosions.end(),
            [](const Explosion& explosion) {
                return !explosion.isAlive();
            }
        ),
        explosions.end()
    );
}

void Game::cleanObjects() {
    for (Enemy& enemy : enemies) {
        if (!enemy.isAlive() && !enemy.reachedBase()) {
            money += enemy.getReward();
        }
    }

    enemies.erase(
        std::remove_if(
            enemies.begin(),
            enemies.end(),
            [](const Enemy& enemy) {
                return !enemy.isAlive();
            }
        ),
        enemies.end()
    );

    bullets.erase(
        std::remove_if(
            bullets.begin(),
            bullets.end(),
            [](const Bullet& bullet) {
                return !bullet.isAlive();
            }
        ),
        bullets.end()
    );
}

void Game::render() {
    window.clear(sf::Color(20, 20, 25));

    if (screenState == ScreenState::MainMenu) {
        drawMainMenu();
        window.display();
        return;
    }

    if (screenState == ScreenState::LevelSelect) {
        drawLevelSelect();
        window.display();
        return;
    }

    if (screenState == ScreenState::Settings) {
        drawSettings();
        window.display();
        return;
    }

    map.draw(window);

    for (Explosion& explosion : explosions) {
        explosion.draw(window);
    }

    for (auto& tower : towers) {
        tower->draw(window);
    }

    for (Enemy& enemy : enemies) {
        enemy.draw(window);
    }

    for (Bullet& bullet : bullets) {
        bullet.draw(window);
    }

    assets.drawEffects(window);
    drawUI();

    if (menuState == MenuState::Build) {
        window.draw(gunBuildButton);
        window.draw(rocketBuildButton);
        window.draw(sniperBuildButton);
        window.draw(gunBuildText);
        window.draw(rocketBuildText);
        window.draw(sniperBuildText);
    }

    if (menuState == MenuState::Tower && selectedTowerIndex >= 0) {
        int cost = towers[selectedTowerIndex]->getUpgradeCost();

        if (cost > 0) {
            upgradeText.setString("Upgrade $" + std::to_string(cost));
        } else {
            upgradeText.setString("MAX");
        }

        sellText.setString("Sell $" + std::to_string(towers[selectedTowerIndex]->getSellValue()));

        window.draw(upgradeButton);
        window.draw(sellButton);
        window.draw(upgradeText);
        window.draw(sellText);
    }

    window.display();
}

void Game::drawMainMenu() {
    drawText("ROBOT TOWER DEFENSE", WINDOW_WIDTH / 2.f - 235.f, 115.f, 42);
    drawText("Main Menu", WINDOW_WIDTH / 2.f - 70.f, 165.f, 22);

    drawButton(playButton, "Play");
    drawButton(settingsButton, "Settings");
    drawButton(exitButton, "Exit");
}

void Game::drawLevelSelect() {
drawText("Choose a level to start", WINDOW_WIDTH / 2.f - 120.f, 170.f, 18);

drawButton(level1Button, "Level 1", 20);
drawButton(level2Button, "Level 2", 20);
drawButton(level3Button, "Level 3", 20);
}
void Game::drawSettings() {
    drawText("SETTINGS", WINDOW_WIDTH / 2.f - 105.f, 125.f, 42);

    drawButton(fpsButton, showFps ? "Show FPS: ON" : "Show FPS: OFF", 20);
    drawButton(frameLimitButton, frameLimitEnabled ? "FPS Limit: 60" : "FPS Limit: OFF", 20);
    drawButton(settingsBackButton, "Back");
}

void Game::drawButton(const sf::RectangleShape& button, const std::string& text, int textSize) {
    window.draw(button);

    sf::FloatRect bounds = button.getGlobalBounds();
    sf::Text textObject(font, text, textSize);
    textObject.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = textObject.getLocalBounds();
    float x = bounds.position.x + bounds.size.x / 2.f - textBounds.size.x / 2.f;
    float y = bounds.position.y + bounds.size.y / 2.f - textBounds.size.y / 2.f - 5.f;

    textObject.setPosition({x, y});
    window.draw(textObject);
}

void Game::drawUI() {
    drawText("HP: " + std::to_string(baseHp), 10, 8, 18);
    drawText("Money: " + std::to_string(money), 10, 32, 18);
    drawText("Level: " + std::to_string(currentLevel), 10, 56, 18);
    drawText("Wave: " + std::to_string(waves.getWave()) + "/" + std::to_string(waves.getMaxWaves()), 10, 80, 18);
    drawText("Space: next wave", 10, 112, 14);
    drawText("Shift+Click: rocket", 10, 132, 14);
    drawText("P: pause", 10, 152, 14);
    drawText("Esc: main menu", 10, 172, 14);
    drawText("Rocket CD: " + std::to_string((int)assets.getRocketCooldown()), 10, 196, 14);

    if (showFps) {
        drawText("FPS: " + std::to_string(currentFps), 10, 220, 14);
    }

    if (paused) {
        drawText("PAUSED", WINDOW_WIDTH / 2.f - 80.f, WINDOW_HEIGHT / 2.f - 40.f, 42);
    }

    if (gameOver) {
        drawText("GAME OVER", WINDOW_WIDTH / 2.f - 120.f, WINDOW_HEIGHT / 2.f - 60.f, 48);
        drawText("Press R to restart", WINDOW_WIDTH / 2.f - 110.f, WINDOW_HEIGHT / 2.f, 28);
        drawText("Press Esc for main menu", WINDOW_WIDTH / 2.f - 135.f, WINDOW_HEIGHT / 2.f + 38.f, 22);
    }

    if (victory) {
        drawText("VICTORY", WINDOW_WIDTH / 2.f - 95.f, WINDOW_HEIGHT / 2.f - 60.f, 48);
        drawText("Press R to restart", WINDOW_WIDTH / 2.f - 110.f, WINDOW_HEIGHT / 2.f, 28);
        drawText("Press Esc for main menu", WINDOW_WIDTH / 2.f - 135.f, WINDOW_HEIGHT / 2.f + 38.f, 22);
    }
}

void Game::drawText(const std::string& text, float x, float y, int size) {
    sf::Text textObject(font, text, size);
    textObject.setFillColor(sf::Color::White);
    textObject.setPosition(sf::Vector2f(x, y));
    window.draw(textObject);
}
void Game::rebuildGameObjectList() {
    gameObjects.clear();

    for (auto& tower : towers) {
        gameObjects.push_back(tower.get());
    }
}