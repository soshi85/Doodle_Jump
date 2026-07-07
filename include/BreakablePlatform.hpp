#pragma once
#include "Platform.hpp"


class BreakablePlatform:public Platform{
private:
    bool broken;
    float fallVelocity;

public:
    BreakablePlatform(const sf::Texture& texture, sf::Vector2f position);

    void update(sf::Time deltaTime) override;
    void onLand(Player& player) override;
    bool isLandable() const override;
};
