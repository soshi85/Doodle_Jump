#include "../include/Monster.hpp"

Monster::Monster(const sf::Texture& texture, sf::Vector2f startPos, int startHealth, MonsterType mType) 
    : sprite(texture), health(startHealth), type(mType) { 
    
    if (type == MonsterType::Blue) {
        velocity = sf::Vector2f{Constants::MONSTER_SPEED, 0.f};
    } 
    else {
        velocity = sf::Vector2f{0.f, 0.f}; 
    }
    
    sprite.setPosition(startPos);
}

void Monster::update(sf::Time deltaTime) {
    float dt = deltaTime.asSeconds();
    sprite.move(sf::Vector2f{velocity.x * dt, velocity.y * dt});

    if (type == MonsterType::Blue) {
        sf::Vector2f pos = sprite.getPosition();
        sf::FloatRect bounds = sprite.getGlobalBounds();

        if (pos.x < 0.f) {
            sprite.setPosition(sf::Vector2f{0.f, pos.y});
            velocity.x = -velocity.x;
        } 
        else if (pos.x + bounds.size.x > static_cast<float>(Constants::WINDOW_WIDTH)) {
    
            sprite.setPosition(sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH) - bounds.size.x, pos.y});
            velocity.x = -velocity.x;
        }
    }
}

void Monster::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Monster::getBounds() const {
    return sprite.getGlobalBounds();
}

void Monster::move(sf::Vector2f offset) {
    sprite.move(offset);
}

bool Monster::takeDamage() {
    health--;
    return health <= 0;
}

int Monster::getHealth() const {
    return health;
}

bool Monster::isOffScreen(float bottomY) const {
    return sprite.getPosition().y > bottomY;
}

sf::Vector2f Monster::getPosition() const {
    return sprite.getPosition();
}