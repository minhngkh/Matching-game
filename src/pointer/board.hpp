#pragma once

#include "global.hpp"
#include "display.hpp"
#include "extra.hpp"

#include <random>
#include <string>

// min = 0, the higher num the less random the board is
#define NON_RANDOMNESS 1

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
#define ST_NOPAIRS 5
#define ST_RESET 6

int GetInput(Card **board, int boardHeight, int boardWidth, Pos *selectedPos, Pos *&path, int &pathLen);

void RemovePair(Card **board, Pos* pair);

void SlideBoard(Card **board, int boardWidth, Pos removedPos);

void SlideBoard(Card **board, int boardWidth, Pos *removedPos);