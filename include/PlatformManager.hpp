#pragma once

#include <vector>
#include <memory>
#include <random>
#include "Platform.hpp"
#include "ResourceManager.hpp"

class Player;

class PlatformManager{
private:
    std::vector<std::unique_ptr<Platform>> platforms;
    ResourceManager<sf::Texture>& textureManager;

    float highestGeneratedY;
    std::mt19937 rng;

    void generatePlatformAbove(float y);

public:
    explicit PlatformManager(ResourceManager<sf::Texture>& textures);

    void reset(sf::Vector2f groundPosition);
    void update(sf::Time deltaTime, float scrollAmount);
    void render(sf::RenderWindow& window);
    void checkCollisions(Player& player);
};
