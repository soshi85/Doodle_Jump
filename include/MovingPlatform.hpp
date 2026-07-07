#pragma once
#include "Platform.hpp"

class MovingPlatform : public Platform {
private:
    float speed;
    int direction;

public:
    MovingPlatform(const sf::Texture& texture, sf::Vector2f position, float speed);
    void update(sf::Time deltaTime) override;
};
