#include "HighScoreManager.hpp"
#include <fstream>

HighScoreManager::HighScoreManager(std::string path):filepath(std::move(path)), highScore(0){
    load();
}

int HighScoreManager::get() const{
    return highScore;
}

bool HighScoreManager::reportScore(int newScore){
    if (newScore > highScore) {
        highScore = newScore;
        save();
        return true;
    }
    return false;
}

void HighScoreManager::load(){
    std::ifstream file(filepath);
    if (file.is_open()){
        file >> highScore;
        if (!file) {
            highScore=0;
        }
    }
}

void HighScoreManager::save() const{
    std::ofstream file(filepath, std::ios::trunc);
    if (file.is_open()) {
        file << highScore;
    }
}
