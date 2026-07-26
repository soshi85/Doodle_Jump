#pragma once
#include <string>
#include <vector>
#include "GameSetting.hpp"

class HighScoreManager{
private:
    std::string filepath;
    std::vector<int> highScores;

    void load();
    void save() const;

public:
    explicit HighScoreManager(std::string path="highscore.txt");

    int get(Difficulty diff) const;
    bool reportScore(int newScore, Difficulty diff);
};
