#include "../include/Hole.hpp"

Hole::Hole(const sf::Texture& texture, sf::Vector2f position, bool large) 
    : sprite(texture), isLarge(large) {
    
        
    sprite.setScale(sf::Vector2f{1.0f, 1.0f}); 

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f{bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    
    sprite.setPosition(position);
}

void Hole::update(sf::Time deltaTime) {}

void Hole::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Hole::getBounds() const {
    return sprite.getGlobalBounds();
}

void Hole::move(sf::Vector2f offset) {
    sprite.move(offset);
}

bool Hole::isOffScreen(float bottomY) const {
    return (sprite.getPosition().y - sprite.getGlobalBounds().size.y / 2.f) > bottomY;
}

sf::Vector2f Hole::getCenter() const {
    return sprite.getPosition();
}