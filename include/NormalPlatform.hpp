#pragma once
#include "Platform.hpp"

class NormalPlatform : public Platform {
public:
    NormalPlatform(const sf::Texture& texture, sf::Vector2f position);
};

