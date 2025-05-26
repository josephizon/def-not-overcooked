#include "LeaderBoardManager.hpp"
#include <fstream>
#include <algorithm>

std::vector<int> levelScores[3];

void LoadLeaderboard(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    int currentLevel = -1;

    while (std::getline(file, line)) {
        if (line.find("LEVEL") != std::string::npos) {
            currentLevel++;
            levelScores[currentLevel].clear();
        } else if (currentLevel >= 0 && currentLevel < 3) {
            levelScores[currentLevel].push_back(std::stoi(line));
        }
    }

    file.close();
}

void SaveLeaderboard(const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (int i = 0; i < 3; ++i) {
        file << "LEVEL" << i + 1 << "\n";
        for (int score : levelScores[i]) {
            file << score << "\n";
        }
    }

    file.close();
}

void AddScoreToLeaderboard(int levelIndex, int score) {
    levelScores[levelIndex].push_back(score);
    std::sort(levelScores[levelIndex].begin(), levelScores[levelIndex].end(), std::greater<int>());

    if (levelScores[levelIndex].size() > 5) {
        levelScores[levelIndex].resize(5);
    }

    SaveLeaderboard("leaderboard.txt");
}