#include "Asset.h"
#include "Enemy.h"
#include "Utils.h"

AssetSystem::AssetSystem() {
    rocketCooldown = 0;
    effectTimer = 0;
    lastExplosion.setRadius(1);
}

void AssetSystem::update(float dt) {
    if (rocketCooldown > 0) rocketCooldown -= dt;
    if (effectTimer > 0) effectTimer -= dt;
}

void AssetSystem::rocketStrike(sf::Vector2f pos, std::vector<Enemy>& enemies) {
    if (rocketCooldown > 0) return;

    for (Enemy& enemy : enemies) {
        if (enemy.isAlive() && distance(pos, enemy.getPosition()) < 90) {
            enemy.takeDamage(130);
        }
    }

    lastExplosion.setRadius(90);
    lastExplosion.setOrigin(sf::Vector2f(90, 90));
    lastExplosion.setPosition(pos);
    lastExplosion.setFillColor(sf::Color(255, 120, 20, 70));
    effectTimer = 0.35f;
    rocketCooldown = 8;
}

void AssetSystem::drawEffects(sf::RenderWindow& window) {
    if (effectTimer > 0) window.draw(lastExplosion);
}

float AssetSystem::getRocketCooldown() const { return rocketCooldown; }
