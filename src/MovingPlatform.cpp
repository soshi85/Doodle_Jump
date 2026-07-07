#include "MovingPlatform.hpp"
#include "Constants.hpp"

MovingPlatform::MovingPlatform(const sf::Texture& texture, sf::Vector2f position, float speedValue)
    : Platform(texture, position), speed(speedValue), direction(1){
}

void MovingPlatform::update(sf::Time deltaTime){
    float dt= deltaTime.asSeconds();
    sprite.move({direction*speed*dt, 0.f});

    sf::FloatRect bounds= getBounds();
    if (bounds.position.x <= 0.f){
        sprite.move({-bounds.position.x, 0.f});
        direction= 1;
    } else if (bounds.position.x+bounds.size.x>= static_cast<float>(Constants::WINDOW_WIDTH)) {
        sprite.move({static_cast<float>(Constants::WINDOW_WIDTH)-(bounds.position.x+bounds.size.x), 0.f});
        direction= -1;
    }

    
}
