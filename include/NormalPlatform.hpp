#pragma once
#include "Platform.hpp"
#include <memory>
#include "Spring.hpp"

class NormalPlatform : public Platform {
private:
    std::unique_ptr<Spring> spring;

public:
    NormalPlatform(const sf::Texture& texture, sf::Vector2f position);

    void update(sf::Time deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void onLand(Player& player) override;
    void move(sf::Vector2f offset) override; 
    
    void attachSpring(std::unique_ptr<Spring> s);
    bool hasSpring() const;
    Spring* getSpring();
};