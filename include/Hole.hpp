#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class Hole {
private:
    sf::Sprite sprite;
    bool isLarge;

public:

    Hole(const sf::Texture& texture, sf::Vector2f position, bool large);

    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    void move(sf::Vector2f offset);
    bool isOffScreen(float bottomY) const;
    
    sf::Vector2f getCenter() const; 
};