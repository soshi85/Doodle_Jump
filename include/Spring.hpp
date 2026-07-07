#pragma once
#include <SFML/Graphics.hpp>

class Platform;

class Spring {
private:
    sf::Sprite sprite;

    static const sf::IntRect NORMAL_RECT;
    static const sf::IntRect PRESSED_RECT;

    float pressedTimer;
    static constexpr float PRESSED_DURATION = 0.15f;

    void applyRect(const sf::IntRect& rect, const Platform& platform);

public:
    Spring(const sf::Texture& texture, const Platform& platform);

    void update(sf::Time dt);
    void render(sf::RenderWindow& window);
    void followPlatform(const Platform& platform);
    void trigger(const Platform& platform);
    sf::FloatRect getBounds() const;
};
