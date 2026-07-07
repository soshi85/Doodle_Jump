#include "Spring.hpp"
#include "Platform.hpp"

const sf::IntRect Spring::NORMAL_RECT{{0, 19}, {25, 33}};
const sf::IntRect Spring::PRESSED_RECT{{0, 0}, {25, 19}};

Spring::Spring(const sf::Texture& texture, const Platform& platform)
    : sprite(texture), pressedTimer(0.f)
{
    applyRect(NORMAL_RECT, platform);
}

void Spring::applyRect(const sf::IntRect& rect, const Platform& platform){
    sprite.setTextureRect(rect);
    sprite.setOrigin({rect.size.x / 2.0f, static_cast<float>(rect.size.y)});
    followPlatform(platform);
}

void Spring::followPlatform(const Platform& platform){
    sf::FloatRect bounds = platform.getBounds();
    sprite.setPosition({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y});
}

void Spring::trigger(const Platform& platform){
    applyRect(PRESSED_RECT, platform);
    pressedTimer = PRESSED_DURATION;
}

void Spring::update(sf::Time dt) {
    float dtSeconds = dt.asSeconds();
    if (pressedTimer > 0.f) {
        pressedTimer -= dtSeconds;
        if (pressedTimer <= 0.f) {
            sprite.setTextureRect(NORMAL_RECT);
            sprite.setOrigin({NORMAL_RECT.size.x / 2.0f, static_cast<float>(NORMAL_RECT.size.y)});
        }
    }
}

void Spring::render(sf::RenderWindow& window){
    window.draw(sprite);
}

sf::FloatRect Spring::getBounds() const{
    return sprite.getGlobalBounds();
}
