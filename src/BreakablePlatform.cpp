#include "BreakablePlatform.hpp"
#include "Player.hpp"
#include "Constants.hpp"

BreakablePlatform::BreakablePlatform(const sf::Texture& texture, sf::Vector2f position)
    : Platform(texture, position), broken(false), fallVelocity(0.f){
}

void BreakablePlatform::update(sf::Time deltaTime){
    if (broken){
        float dt=deltaTime.asSeconds();
        fallVelocity +=Constants::GRAVITY*dt;
        sprite.move({0.f, fallVelocity*dt});
    }
}

void BreakablePlatform::onLand(Player& player){
    if (broken){
        return;
    }
    broken = true;
    fallVelocity = player.getVelocityY(); //we dont call .jump because the platform gets broken
}

bool BreakablePlatform::isLandable() const{
    return !broken;
}
