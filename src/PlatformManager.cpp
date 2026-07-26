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

bool PlatformManager::isSpaceFree(const sf::FloatRect& bounds) const {
    for (const auto& platform : platforms) {
        if (platform->getBounds().findIntersection(bounds).has_value()) {
            return false;
        }
    }
    for (const auto& monster : monsters) {
        if (monster->getBounds().findIntersection(bounds).has_value()) {
            return false;
        }
    }
    for (const auto& hole : holes) {
        if (hole->getBounds().findIntersection(bounds).has_value()) {
            return false;
        }
    }
    return true;
}

void PlatformManager::reset(sf::Vector2f groundPosition) {
    platforms.clear();
    monsters.clear(); 
    holes.clear();

    platforms.push_back(std::make_unique<NormalPlatform>(
        textureManager.get("assets/normal_platform.png"), groundPosition));

    highestGeneratedY = groundPosition.y;

    while (highestGeneratedY > -Constants::GENERATION_MARGIN_ABOVE_SCREEN){
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

    float monsterChance = (currentDiff == Difficulty::Easy) ? 0.05f : 
                          (currentDiff == Difficulty::Hard) ? 0.25f : 0.15f;

    if (y < -300.f && roll(rng) < monsterChance) { 
        
        if (gap > 100.f) {
            float mY = newY + (gap * 0.4f); 
            
            float mX;
            float halfWidth = static_cast<float>(Constants::WINDOW_WIDTH) / 2.f;
            
            if (newX < halfWidth) {
                mX = newX + halfWidth - 50.f; 
            } else {
                mX = newX - halfWidth + 50.f;
            }

            if (mX < 40.f) mX = 40.f;
            if (mX > static_cast<float>(Constants::WINDOW_WIDTH) - 60.f) {
                mX = static_cast<float>(Constants::WINDOW_WIDTH) - 60.f;
            }
            float padding = 30.f;
            sf::FloatRect safeZone(
                sf::Vector2f{mX - padding, mY - padding}, 
                sf::Vector2f{40.f + (padding * 2.f), 40.f + (padding * 2.f)}
            );
            
            if (isSpaceFree(safeZone)) {
                MonsterType mType = (roll(rng) < 0.5f) ? MonsterType::Blue : MonsterType::Green;
                std::string texName = (mType == MonsterType::Blue) ? "assets/BlueMonster.png" : "assets/green_monster.png";
                int mHealth = (currentDiff == Difficulty::Easy) ? 1 : 
                              (currentDiff == Difficulty::Medium) ? 2 : 3;
                monsters.push_back(std::make_unique<Monster>(textureManager.get(texName), sf::Vector2f{mX, mY}, mHealth, mType));
            }
        }
    }
    
    if (currentDiff == Difficulty::Hard && y < -300.f && roll(rng) < 0.20f) { 
        if (gap > 60.f) { 
            float hY = newY + (gap / 2.f); 
            float hX;
            float halfWidth = static_cast<float>(Constants::WINDOW_WIDTH) / 2.f;
            
            if (newX < halfWidth) {
                hX = newX + halfWidth - 40.f; 
            } else {
                hX = newX - halfWidth + 40.f;
            }

            if (hX < 60.f) hX = 60.f;
            if (hX > static_cast<float>(Constants::WINDOW_WIDTH) - 60.f) {
                hX = static_cast<float>(Constants::WINDOW_WIDTH) - 60.f;
            }

            bool isLarge = (roll(rng) < 0.5f);
            float radius = isLarge ? 50.f : 25.f; 
            
            float paddingX = 20.f;       
            float paddingTop = 20.f;     
            float paddingBottom = 300.f; 
            
            sf::FloatRect safeZone(
                sf::Vector2f{hX - radius - paddingX, hY - radius - paddingTop}, 
                sf::Vector2f{(radius + paddingX) * 2.f, (radius * 2.f) + paddingTop + paddingBottom}
            );

            if (isSpaceFree(safeZone)) {
                std::string texName = isLarge ? "assets/hole@2x.png" : "assets/hole.png";
                sf::Texture& holeTex = textureManager.get(texName); 
                holes.push_back(std::make_unique<Hole>(holeTex, sf::Vector2f{hX, hY}, isLarge));
            }
        }
    }
}

void PlatformManager::update(sf::Time deltaTime, float scrollAmount){
    for (auto& platform : platforms) {
        platform->update(deltaTime);
        if (scrollAmount > 0.f) {
            platform->move({0.f, scrollAmount});
        }
    }
    
    for (auto& monster : monsters) {
        monster->update(deltaTime);
        if (scrollAmount > 0.f) {
            monster->move(sf::Vector2f{0.f, scrollAmount});
        }
    }
           
    for (auto& hole : holes) {
        hole->update(deltaTime);
        if (scrollAmount > 0.f) {
            hole->move(sf::Vector2f{0.f, scrollAmount});
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

    monsters.erase(
        std::remove_if(monsters.begin(), monsters.end(),[](const std::unique_ptr<Monster>& m){
                return m->isOffScreen(static_cast<float>(Constants::WINDOW_HEIGHT));}),
                monsters.end());
    
    holes.erase(
        std::remove_if(holes.begin(), holes.end(), [](const std::unique_ptr<Hole>& h){
                return h->isOffScreen(static_cast<float>(Constants::WINDOW_HEIGHT));}),
        holes.end());
        
    while (highestGeneratedY> -Constants::GENERATION_MARGIN_ABOVE_SCREEN){
        generatePlatformAbove(highestGeneratedY);
    }
}

void PlatformManager::render(sf::RenderWindow& window){
    for (auto& platform:platforms){
        platform->render(window);
    }

    for (auto& monster : monsters) {
        monster->render(window);
    }

    for (auto& hole : holes) {
        hole->render(window);
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