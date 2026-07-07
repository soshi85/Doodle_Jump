#include "PlatformManager.hpp"
#include "NormalPlatform.hpp"
#include "MovingPlatform.hpp"
#include "BreakablePlatform.hpp"
#include "Spring.hpp"
#include "Player.hpp"
#include "Constants.hpp"

#include <algorithm>
#include <chrono>

PlatformManager::PlatformManager(ResourceManager<sf::Texture>& textures)
    : textureManager(textures),
      highestGeneratedY(0.f),
      rng(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()))
{
}

void PlatformManager::reset(sf::Vector2f groundPosition) {
    platforms.clear();

    platforms.push_back(std::make_unique<NormalPlatform>(
        textureManager.get("assets/normal_platform.png"), groundPosition));

    highestGeneratedY = groundPosition.y;

    while (highestGeneratedY >-Constants::GENERATION_MARGIN_ABOVE_SCREEN){
        generatePlatformAbove(highestGeneratedY);
    }
}

void PlatformManager::generatePlatformAbove(float y) {
    float maxJumpHeight = (Constants::JUMP_VELOCITY * Constants::JUMP_VELOCITY) / (2.f * Constants::GRAVITY);


    float lastSolidY = y; 
    bool lastIsBreakable = false;

    if (!platforms.empty()) {
        lastIsBreakable = (dynamic_cast<BreakablePlatform*>(platforms.back().get()) != nullptr);
        

        for (auto it = platforms.rbegin(); it != platforms.rend(); ++it) {
            if (dynamic_cast<BreakablePlatform*>(it->get()) == nullptr) {
                lastSolidY = (*it)->getPosition().y;
                break;
            }
        }
    }


    float maxAllowedReach = maxJumpHeight * 0.8f; 
    float currentDistance = lastSolidY - y;
    float maxGap = maxAllowedReach - currentDistance;


    if (maxGap < Constants::PLATFORM_MIN_VERTICAL_GAP + 10.f) {
        maxGap = Constants::PLATFORM_MIN_VERTICAL_GAP + 10.f;
    }

    std::uniform_real_distribution<float> gapDist(Constants::PLATFORM_MIN_VERTICAL_GAP, maxGap);
    std::uniform_real_distribution<float> xDist(40.f, static_cast<float>(Constants::WINDOW_WIDTH) - 40.f);
    std::uniform_real_distribution<float> speedDist(Constants::MOVING_PLATFORM_MIN_SPEED, Constants::MOVING_PLATFORM_MAX_SPEED);
    std::uniform_real_distribution<float> roll(0.f, 1.f);

    float gap = gapDist(rng);
    float newY = y - gap;
    float newX = xDist(rng);
    float typeRoll = roll(rng);

    std::unique_ptr<Platform> platform;


    if (typeRoll < Constants::PROB_MOVING_PLATFORM) {
        platform = std::make_unique<MovingPlatform>(
            textureManager.get("assets/moving_platform.png"), sf::Vector2f{newX, newY}, speedDist(rng));
            
    
    } else if (typeRoll < Constants::PROB_MOVING_PLATFORM + Constants::PROB_BREAKABLE_PLATFORM && !lastIsBreakable) {
        platform = std::make_unique<BreakablePlatform>(
            textureManager.get("assets/broken_platform.png"), sf::Vector2f{newX, newY});
            
    } else {
        platform = std::make_unique<NormalPlatform>(
            textureManager.get("assets/normal_platform.png"), sf::Vector2f{newX, newY});

        if (auto* normalPtr = dynamic_cast<NormalPlatform*>(platform.get())) {
            if (roll(rng) < Constants::PROB_SPRING_ON_NORMAL) {
                normalPtr->attachSpring(
                    std::make_unique<Spring>(textureManager.get("assets/spring_sprite.png"), *normalPtr));
            }
        }
    }

    platforms.push_back(std::move(platform));
    highestGeneratedY = newY;
}

void PlatformManager::update(sf::Time deltaTime, float scrollAmount){
    for (auto& platform : platforms) {
        platform->update(deltaTime);
        if (scrollAmount > 0.f) {
            platform->move({0.f, scrollAmount});
        }
    }

    if (scrollAmount > 0.f){
        highestGeneratedY += scrollAmount;
    }

    platforms.erase(
        std::remove_if(platforms.begin(), platforms.end(),[](const std::unique_ptr<Platform>& p){
                return p->getBounds().position.y >
                       static_cast<float>(Constants::WINDOW_HEIGHT)+Constants::REMOVAL_MARGIN_BELOW_SCREEN;}),
                       platforms.end());

    while (highestGeneratedY> -Constants::GENERATION_MARGIN_ABOVE_SCREEN){
        generatePlatformAbove(highestGeneratedY);
    }
}

void PlatformManager::render(sf::RenderWindow& window){
    for (auto& platform:platforms){
        platform->render(window);
    }
}

void PlatformManager::checkCollisions(Player& player) {
    if (player.getVelocityY()<= 0.f){
        return;
    }

    sf::FloatRect playerBounds = player.getBounds();

    for (auto& platform : platforms){
        if (!platform->isLandable()){
            continue;
        }
        if (playerBounds.findIntersection(platform->getLandingBounds()).has_value()){
            platform->onLand(player);
            break;
        }
    }
}
