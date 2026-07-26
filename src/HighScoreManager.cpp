#include "HighScoreManager.hpp"
#include <fstream>

HighScoreManager::HighScoreManager(std::string path) 
    : filepath(std::move(path)) {
    highScores.assign(3, 0); 
    load();
}

int HighScoreManager::get(Difficulty diff) const {
    return highScores[static_cast<int>(diff)];
}

bool HighScoreManager::reportScore(int newScore, Difficulty diff) {
    int index = static_cast<int>(diff);
    if (newScore > highScores[index]) {
        highScores[index] = newScore;
        save();
        return true;
    }
    return false;
}

void HighScoreManager::load() {
    std::ifstream file(filepath);
    if (file.is_open()) {
        file >> highScores[0] >> highScores[1] >> highScores[2];
        
        if (!file) {
            highScores.assign(3, 0);
        }
    }
}

void HighScoreManager::save() const {
    std::ofstream file(filepath, std::ios::trunc);
    if (file.is_open()) {
        file << highScores[0] << " " << highScores[1] << " " << highScores[2];
    }
}
