#include "Platform.hpp"

Platform::Platform(const sf::Texture& texture, sf::Vector2f position) : sprite(texture) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    sprite.setPosition(position);
}

void Platform::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Platform::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Platform::getPosition() const {
    return sprite.getPosition();
}

void Platform::move(sf::Vector2f offset) {
    sprite.move(offset);
}