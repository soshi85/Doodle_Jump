#include "Platform.hpp"
#include "Player.hpp"

Platform::Platform(const sf::Texture& texture, sf::Vector2f position) : sprite(texture) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    sprite.setPosition(position);
}

void Platform::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Platform::update(sf::Time /*deltaTime*/) {
}

void Platform::onLand(Player& player) {
    player.jump();
}

sf::FloatRect Platform::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::FloatRect Platform::getLandingBounds() const {
    sf::FloatRect bounds = getBounds();
    float xMargin = 15.f;
    
    return sf::FloatRect(
        {bounds.position.x + xMargin, bounds.position.y}, 
        {std::max(0.f, bounds.size.x - (2.f * xMargin)), 10.f}
    );
}
sf::Vector2f Platform::getPosition() const {
    return sprite.getPosition();
}

void Platform::move(sf::Vector2f offset) {
    sprite.move(offset);
}