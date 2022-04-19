#pragma once

#include "global.hpp"
#include "display.hpp"
#include "linked-list.hpp"
#include "extra.hpp"
#include <random>
#include <string>

bool GenerateBoard(List *&board, int height, int width);

void DisplayCard(Card card);

void DisplayBoard(List *board, int boardHeight, int boardWidth);

void RefreshBoard(List *board, int boardHeight);

bool ToggleCard(Card &card);

bool TogglePair(List *board, Pos *pair);

#define ST_NORMAL 0
#define ST_FORCE_OUT 1
#define ST_SURRENDER 2
#define ST_FINISHED 3
#define ST_ASSISTED 4
#define ST_NOPAIRS 5
#define ST_RESET 6

int GetInput(List *board, int boardHeight, int boardWidth, Pos *selectedPos, Path &currPath);

void RemovePair(List *board, Pos *pair);

void SlideBoard(List *board, Pos removedPos);

void SlideBoard(List *board, Pos *removedPos);