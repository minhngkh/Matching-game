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

#define NORMAL 0
#define FORCE_OUT 1
#define SURRENDER 2
#define FINISHED 3

int GetInput(Card **board, int boardHeight, int boardWidth, Pos *selectedPos);

void RemovePair(Card **board, Pos* pair);

void SlideBoard(int **board, int boardHeight, int boardWidth, Pos removedPos);

void SlideBoard(int **board, int boardHeight, int boardWidth, Pos *removedPos);