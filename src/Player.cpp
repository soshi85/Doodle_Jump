#include "Player.hpp"
#include "Constants.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player(const sf::Texture& leftTex, const sf::Texture& rightTex):sprite(rightTex),velocity({0.0f, 0.0f}), highestY(Constants::PLAYER_START_Y), score(0),
      textureLeft(&leftTex), textureRight(&rightTex){
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x/2.0f, bounds.size.y/2.0f});
    sprite.setScale({0.7f, 0.7f});
    sprite.setPosition({static_cast<float>(Constants::WINDOW_WIDTH)/2.0f, Constants::PLAYER_START_Y});
}

void Player::update(sf::Time deltaTime){
    float dt = deltaTime.asSeconds();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
        velocity.x = -Constants::MOVEMENT_SPEED;
        sprite.setTexture(*textureLeft);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
        velocity.x = Constants::MOVEMENT_SPEED;
        sprite.setTexture(*textureRight);
    } else{
        velocity.x=0.0f;
    }

    velocity.y += Constants::GRAVITY*dt;
    sprite.move({velocity.x*dt,velocity.y*dt});

    sf::Vector2f pos=sprite.getPosition();
    if (pos.x < 0){
        sprite.setPosition({static_cast<float>(Constants::WINDOW_WIDTH),pos.y});
    } else if (pos.x > static_cast<float>(Constants::WINDOW_WIDTH)) {
        sprite.setPosition({0, pos.y});
    }

    if (pos.y<highestY) {
        score += static_cast<int>(highestY-pos.y);
        highestY = pos.y;
    }
}

void Player::jump(float power){
    velocity.y = -power;
}

void Player::render(sf::RenderWindow& window){
    window.draw(sprite);
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
