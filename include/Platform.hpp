#pragma once

#include <SFML/Graphics.hpp>

class Platform {
protected:
    sf::Sprite sprite;

public:
    Platform(const sf::Texture& texture, sf::Vector2f position);
    virtual ~Platform() = default;

    virtual void update(sf::Time deltaTime) {}
    
    virtual void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void move(sf::Vector2f offset);
};
