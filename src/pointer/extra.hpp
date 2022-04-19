#pragma once

#include <windows.h>
#include <MMsystem.h>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>

struct Stat{
    std::string name;
    int time;
};

void WinSound();
void LoseSound();
void CorrectSound();
void ErrorSound();
void MovingSound();

void Swap(Stat &x, Stat &y);
void SortAscendingOrder(Stat *a, int n);
void UpdateLeaderboard(Stat player);
Stat *ReadLeaderboard();

typedef std::chrono::_V2::system_clock::time_point Time;

Time GetCurrTime();

int ElapsedTime(Time end, Time start);