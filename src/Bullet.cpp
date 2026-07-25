#include "Bullet.hpp"


Bullet::Bullet(const sf::Texture& texture, sf::Vector2f startPos) 
    : sprite(texture), velocity(sf::Vector2f{0.f, -Constants::BULLET_SPEED}) { 
    
    sf::FloatRect bounds = sprite.getLocalBounds();
    if (bounds.size.x > 0) {
        float targetWidth = 15.f;
        float scale = targetWidth / bounds.size.x;
        sprite.setScale(sf::Vector2f{scale, scale});
    }

    sprite.setOrigin(sf::Vector2f{bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    sprite.setPosition(startPos);
}

void Bullet::update(sf::Time deltaTime) {
    float dt = deltaTime.asSeconds();
    sprite.move({velocity.x * dt, velocity.y * dt});
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Bullet::getBounds() const {
    return sprite.getGlobalBounds();
}

void Bullet::move(sf::Vector2f offset) {
    sprite.move(offset);
}

bool Bullet::isOffScreen() const {
    return sprite.getPosition().y < 0.f;
}