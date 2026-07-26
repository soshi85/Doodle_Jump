#include "GameSetting.hpp"
#include <fstream>
#include <algorithm>

GameSettings::GameSettings(const std::string& filepath) 
    : filename(filepath), volume(50.f), difficulty(Difficulty::Medium) {
    load();
}

void GameSettings::load() {
    std::ifstream file(filename);
    if (file.is_open()) {
        int diffInt;
        
        if (file >> volume >> diffInt) {
            difficulty = static_cast<Difficulty>(diffInt);
        }
        file.close();
    }
}

void GameSettings::save() const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << volume << " " << static_cast<int>(difficulty) << "\n";
        file.close();
    }
}

float GameSettings::getVolume() const { return volume; }
void GameSettings::setVolume(float v) { volume = std::clamp(v, 0.f, 100.f); }

Difficulty GameSettings::getDifficulty() const { return difficulty; }
void GameSettings::setDifficulty(Difficulty diff) { difficulty = diff; }