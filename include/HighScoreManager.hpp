#pragma once
#include <string>

class HighScoreManager{
private:
    std::string filepath;
    int highScore;

    void load();
    void save() const;

public:
    explicit HighScoreManager(std::string path="highscore.txt");

    int get() const;
    bool reportScore(int newScore);
};
