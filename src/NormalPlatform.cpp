#include "NormalPlatform.hpp"
#include "Player.hpp"
#include "Constants.hpp"

NormalPlatform::NormalPlatform(const sf::Texture& texture, sf::Vector2f position)
    : Platform(texture, position), spring(nullptr) {
}

void NormalPlatform::render(sf::RenderWindow& window) {
    Platform::render(window);
    if (spring) {
        spring->render(window);
    }
}

void NormalPlatform::update(sf::Time deltaTime) {
    Platform::update(deltaTime);
    if (spring) {
        spring->update(deltaTime);
        spring->followPlatform(*this);
    }
}

void NormalPlatform::onLand(Player& player) {
    if (spring && spring->getBounds().findIntersection(player.getBounds()).has_value()) {
        spring->trigger(*this);
        player.jump(Constants::SPRING_JUMP_VELOCITY);
    } else {
        Platform::onLand(player);
    }
}

void NormalPlatform::move(sf::Vector2f offset) {
    Platform::move(offset); 
    if (spring) {
        spring->followPlatform(*this);
    }
}

void NormalPlatform::attachSpring(std::unique_ptr<Spring> s) {
    spring = std::move(s);
    if (spring) {
        spring->followPlatform(*this);
    }
}

bool NormalPlatform::hasSpring() const {
    return spring != nullptr;
}

Spring* NormalPlatform::getSpring() {
    return spring.get();
}