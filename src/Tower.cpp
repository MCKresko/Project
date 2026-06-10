#include <cmath>
#include "Tower.h"
#include "Enemy.h"
#include "Utils.h"

static float getAngle(sf::Vector2f from, sf::Vector2f to) {
    float dx = to.x - from.x;
    float dy = to.y - from.y;

    return std::atan2(dy, dx) * 180.f / 3.14159265f;
}

Tower::Tower(std::string type, sf::Vector2f position)
    : type(type), level(1), timer(0.f), barrelAngle(0.f)
{
    body.setRadius(15.f);
    body.setOrigin({15.f, 15.f});
    body.setPosition(position);
    body.setFillColor(sf::Color(90, 90, 100));
    body.setOutlineThickness(2.f);
    body.setOutlineColor(sf::Color::Black);

    barrel.setSize({32.f, 8.f});
    barrel.setOrigin({4.f, 4.f});
    barrel.setPosition(position);
    barrel.setFillColor(sf::Color(30, 30, 35));
}

void Tower::update(float dt, std::vector<Enemy>& enemies, std::vector<Bullet>& bullets) {
    timer += dt;

    Enemy* target = findTarget(enemies);

    if (target != nullptr) {
        float targetAngle = getAngle(body.getPosition(), target->getPosition());

        float diff = targetAngle - barrelAngle;

        while (diff > 180.f) diff -= 360.f;
        while (diff < -180.f) diff += 360.f;

        float rotationSpeed = 540.f * dt;

        if (std::abs(diff) < rotationSpeed) {
            barrelAngle = targetAngle;
        } else {
            barrelAngle += diff > 0 ? rotationSpeed : -rotationSpeed;
        }

        barrel.setRotation(sf::degrees(barrelAngle));

        float finalDiff = targetAngle - barrelAngle;

        while (finalDiff > 180.f) finalDiff -= 360.f;
        while (finalDiff < -180.f) finalDiff += 360.f;

        float aimTolerance = 10.f;

        if (std::abs(finalDiff) <= aimTolerance && timer >= fireRate) {
            shoot(bullets, target);
            timer = 0.f;
        }
    }
}

Enemy* Tower::findTarget(std::vector<Enemy>& enemies) {
    for (Enemy& enemy : enemies) {
        if (enemy.isAlive() && distance(body.getPosition(), enemy.getPosition()) <= range) {
            return &enemy;
        }
    }

    return nullptr;
}

void Tower::draw(sf::RenderWindow& window) {
    sf::CircleShape rangeCircle(range);
    rangeCircle.setOrigin(sf::Vector2f(range, range));
    rangeCircle.setPosition(body.getPosition());
    rangeCircle.setFillColor(sf::Color(255, 255, 255, 18));
    rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 45));
    rangeCircle.setOutlineThickness(1);
    window.draw(rangeCircle);

    window.draw(barrel);
    window.draw(body);
}

bool Tower::contains(sf::Vector2f point) const {
    sf::Vector2f center = body.getPosition();

    float dx = point.x - center.x;
    float dy = point.y - center.y;

    float radius = body.getRadius() * body.getScale().x;

    return dx * dx + dy * dy <= radius * radius;
}

void Tower::upgrade() {
    if (level >= 5) {
        return;
    }

    level++;

    damage *= 1.15f;
    range += 8;

    if (fireRate > 0.30f) {
        fireRate *= 0.93f;
    }

    float scale = 1.f + (level - 1) * 0.02f;
    body.setScale({scale, scale});
}

sf::Vector2f Tower::getPosition() const { return body.getPosition(); }
int Tower::getLevel() const { return level; }
int Tower::getSellValue() const { return costOf(type) / 2 + level * 15; }

int Tower::getUpgradeCost() const {
    if (level >= 5) {
        return 0;
    }
    return 40 + level * 35;
}

std::string Tower::getType() const { return type; }

int Tower::costOf(std::string type) {
    if (type == "rocket") return 90;
    if (type == "sniper") return 110;
    return 60;
}

// =====================
// GunTower
// =====================

GunTower::GunTower(sf::Vector2f position)
    : Tower("gun", position)
{
    setupType();
}

void GunTower::setupType() {
    range = 100;
    damage = 25;
    fireRate = 0.55f;
    body.setFillColor(sf::Color(100, 100, 220));
}

void GunTower::shoot(std::vector<Bullet>& bullets, Enemy* target) {
    bullets.push_back(Bullet(body.getPosition(), target->getId(), damage, 380));
}

// =====================
// RocketTower
// =====================

RocketTower::RocketTower(sf::Vector2f position)
    : Tower("rocket", position)
{
    setupType();
}

void RocketTower::setupType() {
    range = 150;
    damage = 45;
    fireRate = 1.1f;
    body.setFillColor(sf::Color(220, 120, 40));
}

void RocketTower::shoot(std::vector<Bullet>& bullets, Enemy* target) {
    bullets.push_back(Bullet(body.getPosition(), target->getId(), damage, 320, true, 70.f, 0.20f));
}

// =====================
// SniperTower
// =====================

SniperTower::SniperTower(sf::Vector2f position)
    : Tower("sniper", position)
{
    setupType();
}

void SniperTower::setupType() {
    range = 250;
    damage = 90;
    fireRate = 1.9f;
    body.setFillColor(sf::Color(80, 170, 80));
}

void SniperTower::shoot(std::vector<Bullet>& bullets, Enemy* target) {
    bullets.push_back(Bullet(body.getPosition(), target->getId(), damage, 520));
}