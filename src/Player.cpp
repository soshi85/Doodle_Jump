#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>

const float GRAVITY = 1200.0f;
const float MOVEMENT_SPEED = 300.0f;
const float WINDOW_WIDTH = 400.0f;

Player::Player(const sf::Texture& leftTex, const sf::Texture& rightTex) 
    : sprite(rightTex), 
      velocity({0.0f, 0.0f}), score(0), highestY(500.0f),
      textureLeft(&leftTex), textureRight(&rightTex) {
    
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    sprite.setPosition({WINDOW_WIDTH / 2.0f, 500.0f});
}

void Player::update(sf::Time deltaTime) {
    float dt = deltaTime.asSeconds();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -MOVEMENT_SPEED;
        sprite.setTexture(*textureLeft);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = MOVEMENT_SPEED;
        sprite.setTexture(*textureRight);
    } else {
        velocity.x = 0.0f; 
    }

    velocity.y += GRAVITY * dt;
    sprite.move({velocity.x * dt, velocity.y * dt});

    sf::Vector2f pos = sprite.getPosition();
    if (pos.x < 0) {
        sprite.setPosition({WINDOW_WIDTH, pos.y});
    } else if (pos.x > WINDOW_WIDTH) {
        sprite.setPosition({0, pos.y});
    }

    if (pos.y < highestY) {
        score += static_cast<int>(highestY - pos.y);
        highestY = pos.y;
    }
}

void Player::jump(float power) {
    velocity.y = -power;
}

void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

float Player::getVelocityY() const {
    return velocity.y;
}

void Player::move(sf::Vector2f offset) {
    sprite.move(offset);
}

int Player::getScore() const {
    return score;
}