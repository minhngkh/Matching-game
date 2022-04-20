#pragma once

#include <windows.h>
#include <MMsystem.h>
#include <string>
#include <fstream>
#include <chrono>

struct Stat{
    char name[10];
    int time;
};

void WinSound();
void LoseSound();
void CorrectSound();
void ErrorSound();
void MovingSound();

#define ORG_PATH "data/leaderboard"

void Swap(Stat &x, Stat &y);
void SortAscendingOrder(Stat *a, int n);
void UpdateLeaderboard(Stat player, int height, int width);
Stat *ReadLeaderboard(int height, int width, int &size);

typedef std::chrono::_V2::system_clock::time_point Time;

Time GetCurrTime();

int ElapsedTime(Time end, Time start);