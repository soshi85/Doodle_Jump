#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Player;

class Platform {
protected:
    sf::Sprite sprite;

public:
    Platform(const sf::Texture& texture, sf::Vector2f position);
    virtual ~Platform() = default;

    virtual void update(sf::Time deltaTime);
    virtual void render(sf::RenderWindow& window);
    virtual void onLand(Player& player);

    virtual bool isLandable() const { return true; }

    sf::FloatRect getBounds() const;
    sf::FloatRect getLandingBounds() const;
    sf::Vector2f getPosition() const;
    
    virtual void move(sf::Vector2f offset); 
};