#pragma once

#include <vector>
#include <memory>
#include <random>
#include "Platform.hpp"
#include "ResourceManager.hpp"
#include "../include/Monster.hpp"

class Player;

class PlatformManager{
private:
    std::vector<std::unique_ptr<Platform>> platforms;
    std::vector<std::unique_ptr<Monster>> monsters; 

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


    bool isSpaceFree(const sf::FloatRect& bounds) const;

    std::vector<std::unique_ptr<Monster>>& getMonsters() { return monsters; }
};
