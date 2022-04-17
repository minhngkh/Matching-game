#pragma once

#include "global.hpp"
#include "display.hpp"
#include <random>
#include <string>

bool GenerateBoard(Card **&board, int height, int width);

void DisplayCard(Card card);

void DisplayBoard(Card **board, int boardHeight, int boardWidth);

void RefreshBoard(Card **board, int boardHeight, int boardWidth);

bool ToggleCard(Card &card);

bool TogglePair(Card **board, Pos *pair);

#define ST_NORMAL 0
#define ST_FORCE_OUT 1
#define ST_SURRENDER 2
#define ST_FINISHED 3
#define ST_ASSISTED 4

int GetInput(Card **board, int boardHeight, int boardWidth, Pos *selectedPos, Pos *&path, int &pathLen);

void RemovePair(Card **board, Pos* pair);

void SlideBoard(Card **board, int boardWidth, Pos removedPos);

void SlideBoard(Card **board, int boardWidth, Pos *removedPos);