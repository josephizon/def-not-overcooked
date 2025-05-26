#ifndef LEADERBOARDMANAGER_HPP
#define LEADERBOARDMANAGER_HPP
#include <vector>
#include <string>
#include "raylib.h"


extern std::vector<int> levelScores[3];

void LoadLeaderboard(const char* filename);
void SaveLeaderboard(const char* filename);
void AddScoreToLeaderboard(int levelIndex, int score);

#endif