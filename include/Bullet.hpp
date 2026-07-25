#pragma once
#include "Constants.hpp"
#include <SFML/Graphics.hpp>

class Bullet {
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;

public:
    Bullet(const sf::Texture& texture, sf::Vector2f startPos);

    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    void move(sf::Vector2f offset);
    bool isOffScreen() const;
};