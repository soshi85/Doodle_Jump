#include "../include/Player.hpp"
#include "../include/Constants.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player(const sf::Texture& leftTex, const sf::Texture& rightTex, const sf::Texture& shootTex, const sf::Texture& snoutTex)
    : sprite(rightTex), 
      velocity(sf::Vector2f{0.0f, 0.0f}), 
      textureShoot(&shootTex), 
      fireCooldown(0.f), 
      isShootingState(false), 
      highestY(Constants::PLAYER_START_Y), 
      score(0),
      textureLeft(&leftTex), 
      textureRight(&rightTex),
      snoutSprite(snoutTex) { 
    
    sprite.setScale(sf::Vector2f{0.6f, 0.6f}); 
    
    snoutSprite.setScale(sf::Vector2f{0.6f, 0.6f}); 

    sf::FloatRect bounds = sprite.getGlobalBounds();
    sprite.setPosition(sf::Vector2f{Constants::WINDOW_WIDTH / 2.f - bounds.size.x / 2.f, Constants::PLAYER_START_Y - bounds.size.y});
}

void Player::update(sf::Time deltaTime){
    float dt = deltaTime.asSeconds();
    
    if (fireCooldown > 0.f) {
        fireCooldown -= dt;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -Constants::MOVEMENT_SPEED;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = Constants::MOVEMENT_SPEED;
    } else {
        velocity.x = 0.0f;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        isShootingState = true;
        sprite.setTexture(*textureShoot, true);
        sprite.setScale(sf::Vector2f{1.2f, 1.2f});
        
        sf::FloatRect bodyBounds = sprite.getGlobalBounds();
        sf::FloatRect snoutBounds = snoutSprite.getGlobalBounds();
        
        float snoutX = bodyBounds.position.x + (bodyBounds.size.x / 2.f) - (snoutBounds.size.x / 2.f);
        float snoutY = bodyBounds.position.y - snoutBounds.size.y + 75.f; 
        
        snoutSprite.setPosition(sf::Vector2f{snoutX, snoutY});
        
    } else {
        isShootingState = false;
        if (velocity.x < 0) {
            sprite.setTexture(*textureLeft, true);
        } else {
            sprite.setTexture(*textureRight, true); 
        }
        sprite.setScale(sf::Vector2f{0.6f, 0.6f}); 
    }

    velocity.y += Constants::GRAVITY * dt;
    sprite.move(sf::Vector2f{velocity.x * dt, velocity.y * dt});

    sf::Vector2f pos = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();
    
    if (pos.x + bounds.size.x < 0.f) {
        sprite.setPosition(sf::Vector2f{static_cast<float>(Constants::WINDOW_WIDTH), pos.y});
    } else if (pos.x > static_cast<float>(Constants::WINDOW_WIDTH)) {
        sprite.setPosition(sf::Vector2f{-bounds.size.x, pos.y});
    }

    if (pos.y < highestY) {
        score += static_cast<int>(highestY - pos.y);
        highestY = pos.y;
    }
}

void Player::jump(float power){
    velocity.y = -power;
}

void Player::render(sf::RenderWindow& window){
    window.draw(sprite);
    if (isShootingState) {
        window.draw(snoutSprite);
    }
}


sf::FloatRect Player::getBounds() const{
    return sprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const{
    return sprite.getPosition();
}

float Player::getVelocityY() const{
    return velocity.y;
}

void Player::move(sf::Vector2f offset){
    sprite.move(offset);
    highestY += offset.y; 
}

int Player::getScore() const{
    return score;
}

bool Player::wantsToShoot() {
    if (isShootingState && fireCooldown <= 0.f) {
        fireCooldown = Constants::FIRE_RATE_MEDIUM; 
        return true;
    }
    return false;
}