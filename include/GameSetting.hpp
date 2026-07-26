#pragma once
#include <string>

enum class Difficulty {
    Easy,
    Medium,
    Hard
};

class GameSettings {
private:
    float volume;
    Difficulty difficulty;
    std::string filename;

public:
    GameSettings(const std::string& filepath);
    
    void load();
    void save() const;

    float getVolume() const;
    void setVolume(float v);

    Difficulty getDifficulty() const;
    void setDifficulty(Difficulty diff);
};