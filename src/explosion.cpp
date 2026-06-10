#include "Explosion.h"

Explosion::Explosion(sf::Vector2f position, float radius) {
    maxLifetime = 0.5f;
    lifetime = maxLifetime;
    maxRadius = radius;

    shape.setRadius(radius);
    shape.setOrigin({radius, radius});
    shape.setPosition(position);

    shape.setFillColor(sf::Color(255, 0, 0, 150));
    shape.setOutlineColor(sf::Color(255, 255, 0, 255));
    shape.setOutlineThickness(1.f);
}

void Explosion::update(float dt) {
    lifetime -= dt;

    float progress = 1.f - lifetime / maxLifetime;

    if (progress < 0.f) progress = 0.f;
    if (progress > 1.f) progress = 1.f;

    float radius = maxRadius * progress;

    shape.setRadius(radius);
    shape.setOrigin({radius, radius});

    sf::Color fill = shape.getFillColor();
    sf::Color outline = shape.getOutlineColor();

    fill.a = static_cast<std::uint8_t>(90 * (1.f - progress));
    outline.a = static_cast<std::uint8_t>(180 * (1.f - progress));

    shape.setFillColor(fill);
    shape.setOutlineColor(outline);
}

void Explosion::draw(sf::RenderWindow& window) {
    if (isAlive()) {
        window.draw(shape);
    }
}

bool Explosion::isAlive() const {
    return lifetime > 0.f;
}